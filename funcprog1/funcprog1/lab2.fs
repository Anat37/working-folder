module lab2

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
    | OpenBrace
    | CloseBrace
    | String of string
    | OpenSquareBrace
    | CloseSquareBrace
    | Colon
    | Comma


let rec explode = 
    

let parse source = 
    let rec parse' json =
        let rec parseObject (obj:(string*JSON) list) = function
            |Token.String name::Colon::ts->
                let (parsed, ts') = parse' ts
                parseObject ((name,parsed)::obj) ts
            | Token.Comma::ts ->parseObject obj ts
            | Token.CloseBrace::ts -> (JSON.Object obj, ts) 
        match json with 
        | Token.Null::t -> JSON.Null, t
        | Token.OpenBrace::t -> parseObject [] t  
    match parse' source with 
    res, [] -> res
    | _, _ -> failwith "Mailformed token list"

let lab3 = function
  | Object list -> 0

let stringify = function
  | Object list -> "{}"

let generate = 
  let rnd = new Random()
  match rnd.Next(42) with
    | 0 -> Object []
    | _ -> Object [("random", Object [])]

let main () = 
  let values = new NameValueCollection()
  values.Add("email", email)
  values.Add("content", File.ReadAllText(__SOURCE_DIRECTORY__ + @"/" + __SOURCE_FILE__))

  let client = new WebClient()
  let response = client.UploadValues(new Uri("http://91.239.142.110:13666/lab2"), values)
  let responseString = Text.Encoding.Default.GetString(response)

  printf "%A\n" responseString
