module lab

open System
open System.IO
open System.Net
open System.Text
open System.Collections.Specialized

// почтовый адрес
let email = "kozlof9@yandex.ru"


type JSON = 
    Null 
    | Boolean of bool 
    | String of string 
    | Number of int 
    | Array of JSON list 
    | Object of (string * JSON) list

type Token =
    Null
    | True
    | False
    | OpenBrace
    | CloseBrace
    | OpenSBrace
    | CloseSBrace
    | String of string
    | Number of string
    | Colon
    | Comma


let explode (s:string) = [for c in s -> c] 
    
let rec tokenize = 
    let rec parseString (acc:string) =
        function 
        | '"'::xs -> Token.String(acc)::tokenize xs
        | (a:char)::xs -> parseString (acc + string(a)) xs
        | [] -> failwith "Bad format string"
    let rec parseNumber (acc:string) =
        function 
        | ','::xs -> Token.Number(acc)::Comma::tokenize xs
        | '}'::xs -> Token.Number(acc)::CloseBrace::tokenize xs
        | ']'::xs -> Token.Number(acc)::CloseSBrace::tokenize xs
        | (a:char)::xs -> parseNumber (acc + string(a)) xs
        | [] -> failwith "Bad format"
    function
    | 'n'::'u'::'l'::'l'::xs -> Token.Null::(tokenize xs)
    | 't'::'r'::'u'::'e'::xs -> Token.True::(tokenize xs)
    | 'f'::'a'::'l'::'s'::'e'::xs -> Token.False::(tokenize xs)
    | '{'::xs -> OpenBrace::(tokenize xs)
    | '}'::xs -> CloseBrace::(tokenize xs)
    | '['::xs -> OpenSBrace::(tokenize xs)
    | ']'::xs -> CloseSBrace::(tokenize xs)
    | ':'::xs -> Colon::(tokenize xs)
    | ','::xs -> Comma::(tokenize xs)
    | '"' ::xs -> parseString "" xs
    | (' ' | '\n')::xs -> tokenize xs
    | x::xs -> parseNumber (string(x)) xs
    | [] -> []

let parseJson source = 
    let rec parseObject (obj:(string*JSON) list) = 
        let rec parseMember =
            let rec parseValue =
                let rec parseArray acc = function
                    | Token.CloseSBrace::ts -> (JSON.Array acc, ts)
                    | (Token.Comma | Token.OpenSBrace)::ts -> 
                        let (parsed, ts') = parseValue ts
                        parseArray (parsed::acc) ts'
                function 
                | Token.String str::ts -> (JSON.String str, ts)
                | Token.Number str::ts -> (JSON.Number (int str), ts)
                | Token.True::ts -> (JSON.Boolean true, ts)
                | Token.False::ts -> (JSON.Boolean false, ts)
                | Token.OpenBrace::ts -> 
                    let (parsed, ts') = parseObject [] (Token.OpenBrace::ts)
                    (parsed, ts')
                | Token.OpenSBrace::ts -> 
                    let (parsed, ts') = parseArray [] (Token.OpenSBrace::ts)
                    (parsed, ts')
                | Token.Null::ts -> (JSON.Null, ts)
            function
            | Token.String name::Colon::ts -> 
                let (parsed, ts') = parseValue ts
                ((name, parsed), ts')
        function
        | CloseBrace::ts -> (JSON.Object obj, ts)
        | (OpenBrace | Comma)::ts -> 
            let (parsed, ts') = parseMember ts
            parseObject (parsed::obj) ts'                
    match parseObject [] source with 
    res, [] -> res
    | _, _ -> failwith "Mailformed token list"


let rec lab2 = function 
    | JSON.Array(x::[]) -> (lab2 x) + 1 
    | JSON.Object((name, obj)::[])-> (lab2 obj) + 1
    | JSON.Array(x::xs) -> (lab2 x) + (lab2 (JSON.Array xs)) 
    | JSON.Object((name, obj)::xs)-> (lab2 obj) + (lab2 (JSON.Object xs)) 
    | _ -> 0

let rec stringify = 
  let rec stringObject acc =
    function
    | (name, obj)::[] -> acc + "\"" + name + "\"" + ":" + (stringify obj) + "}"
    | (name, obj)::xs -> stringObject (acc + "\"" + name + "\":" + (stringify obj) + ",") xs
    | [] -> "}"
  let rec stringArray acc =
    function
    | x::[] -> acc + (stringify x) + "]"
    | x::xs -> stringArray (acc + (stringify x) + ",") xs
    | [] -> "]"
  function
  | JSON.Object(xs) -> "{" + (stringObject "" xs)
  | JSON.Array(xs) -> "[" + (stringArray "" xs)
  | JSON.Boolean(x) -> if x = true then "true" else "false"
  | JSON.Null -> "null"
  | JSON.Number(x) -> string(x)
  | JSON.String(x) -> "\"" + x + "\""



let rec generate() =
  let rnd = new Random()
  let rec generateString acc =
     match rnd.Next(8) with
     | 1 -> generateString (acc + "a")
     | 2 -> generateString (acc + "b")
     | 3 -> generateString (acc + "c")
     | 4 -> generateString (acc + "d")
     | 5 -> generateString (acc + "e")
     | 6 -> generateString (acc + "f")
     | _ -> acc
  let rec generateArray acc = 
    match rnd.Next(1) with
    | 0 -> JSON.Array acc
    | _ -> generateArray (generate()::acc)  
  let rec generateObject acc = 
    match rnd.Next(2) with
    | 0 -> JSON.Object acc
    | _ -> generateObject ((generateString "", generate())::acc)
  match rnd.Next(20) with
    | 0 -> JSON.Null
    | 1 -> JSON.String (generateString "")
    | 2 -> Boolean(true)
    | 3 -> Boolean(false)
    | 4 -> generateArray []
    | 5 -> JSON.Number(rnd.Next(42))
    | _ -> generateObject []

let jsonExample = "{
  \"Comment\":\"My comment\",
  \"Count\":10,
  \"DiskParam\":
  {
    \"DB\":10,
    \"DBAngle\":1
  },
  \"Range\":true,
  \"Blades\":
  [
    {
      \"Caption\":\"A\",
      \"Value\":65
    },
    {
      \"Caption\":\"B\",
      \"Value\":66
    },
    {
      \"Caption\":\"C\",
      \"Value\":67
    }
  ],
  \"Slots\":
  [
    0,1,2
  ]
}"

let test1() = jsonExample |> explode |> tokenize |> parseJson

let test2() = test1() |> stringify

let test3() = (lab2 <| test1()) = 7

let test4() = lab2 <| generate()
     

let main () = 
  let values = new NameValueCollection()
  values.Add("email", email)
  values.Add("content", File.ReadAllText(__SOURCE_DIRECTORY__ + @"/" + __SOURCE_FILE__))

  let client = new WebClient()
  let response = client.UploadValues(new Uri("http://91.239.142.110:13666/lab2"), values)
  let responseString = Text.Encoding.Default.GetString(response)

  printf "%A\n" responseString

main()