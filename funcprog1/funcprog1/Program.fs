open System
open System.Net
open System.IO
open System.Collections.Specialized

// почтовый адрес
let email = "kozlof9@yandex.ru"
// общий тип для возвращаемых вашими функциями значений, где первая часть кортежа это само значение функции, вторая - кол-во операций
type Result = float * int
let delta = 1e-10

// *** Первая часть

let fTailor x : float = x*(3. - x)/((1. - x)**3.) // функция, которую раскладываем
let n, a, b = 20., 0., 0.5 // интервал

let tailor x : Result =
    let rec calc accmember accres k: Result =
        if ((accres - fTailor x) < delta) then (accres, int(k))
        else calc (x * accmember) (accres + (k * (k + 2.) * x * accmember)) (k + 1.)
    calc 1. 0. 1. 

let tailorA x : Result =
    let nMember k : float = 
        k*(k + 2.)*(x**k)
    let rec calc accres k: Result =
        if (abs(accres - fTailor x) < delta) then (accres, int(k))
        else calc (accres + nMember k) (k + 1.)
    calc 0. 1. 




let printTailor () = 
    [a .. (b-a)/n .. b] 
    |> List.map (fun x -> let (firstRes, firstCou), (secondRes, secondCou) = tailor x, tailorA x in (x, firstRes, firstCou, secondRes, secondCou, fTailor x))
    |> List.iter (fun (a,b,c,d,e,f) -> printf "%f\t%f\t%d\t%f\t%d\t%f\n" a b c d e f )

// *** Вторая часть

let fSolve = fun x -> x // функция, решение которой ищем

let firstFunc x : float = 3.*(log x)**2. + 6. * (log x) - 5.
let secondFunc x : float = 0.6 * (3. **x) - 2.3 * x - 3.
let thirdFunc x : float = x**2. - (log (1. + x)) - 3.

let firstFuncI x : float = x - 0.2 * firstFunc x
let secondFuncI x : float = x - 0.2 * secondFunc x
let thirdFuncI x : float = x - 0.05 * thirdFunc x

let firstFuncD x : float = 6.*(log x) / x + 6. / x
let secondFuncD x : float = 0.6*(3.**x)/ (log 3.) - 2.3
let thirdFuncD x : float = 2.*x - 1./(1. + x)

let firstFuncN x : float = x - (firstFunc x) / (firstFuncD x)
let secondFuncN x : float = x - (secondFunc x) / (secondFuncD x)
let thirdFuncN x : float = x - (thirdFunc x) / (thirdFuncD x)

let iter (f:float -> float) (a:float) (b:float) : Result = 
    let rec iter' x n : Result =
        if (n > 100) then (-1., 0)
        elif (abs(x - f x) < delta) 
            then (f x, n + 1)
            else iter' (f x) (n + 1)
    iter' ((a + b)/2.) 0
    
let newton f a b : Result = 
    iter f a b

let dichotomy =
    // для функций с аккумулятором удобно ставить его в начало
    let rec dichotomyA i (f:float->float) (a:float) (b:float) : Result =
        if (abs(b - a) < delta) 
            then ((a+b)/2., i)
        elif ((f a > 0.) <> (f ((a+b)/2.) > 0.)) 
            then dichotomyA (i + 1) f a ((a+b)/2.) 
            else dichotomyA (i + 1) f ((a+b)/2.) b 
    dichotomyA 0 // чтобы воспользоваться каррированием

let methods : ((float->float)->float->float->Result)list = dichotomy::iter::newton::[]
let bounds : (float*float)list = (1.,3.)::(2.,3.)::(2.,3.)::[]

let rec map (ms:((float->float)->float->float->Result)list) (bs:(float*float)list) (fs:(float->float)list) : (Result)list =
    match (ms, fs, bs) with
    | x::xs, y::ys, (a , b)::zs-> (x y a b)::map xs zs ys
    | _ -> []

let printSolve () =
    (firstFunc::firstFuncI::firstFuncN::[])::(secondFunc::secondFuncI::secondFuncN::[])::(thirdFunc::thirdFuncI::thirdFuncN::[])::[]
    |> List.map (fun f -> map methods bounds f) 
    |> List.map (List.iter (fun (res, cou) -> printf "%f\t%d\n" res cou))

let main () = 
  let values = new NameValueCollection()
  values.Add("email", email)
  values.Add("content", File.ReadAllText(__SOURCE_DIRECTORY__ + @"/" + __SOURCE_FILE__))

  let client = new WebClient()
  let response = client.UploadValues(new Uri("http://91.239.142.110:13666/lab1"), values)
  let responseString = Text.Encoding.Default.GetString(response)

  printf "%A\n" responseString

main()