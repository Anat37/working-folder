6 / -5
2 * 2 <> 5 // val it : bool = true
2 = "два"
333333333333333333333333333333333333333333333333333333333333333I
6. / 5. 
6. / 5. // val it : float = 1.2
42 + 3.4
1.1 + (float 2)
9. - v
2. + max 2.5 5.6 + min 3.4 (max 2.3 4.5)
2 - 2 - 2
let inc x = x + 1
inc 1
let dec x = x - 1
// let dec x = x - 1. // -.
let rec fac n = if n = 0 then 1 else n * fac (dec n)
fac 5
let square x = x * x
let squareIfLess5 x = if x < 5 then square x else x
let square (x:float) = x * x
let squareIfLess5 x = if x < 5. then square x else x
let m = "Марьванна" // val it : string = "Марьванна"
let h = "Hello"
let hm  = h + " " + m
(-) 3 4
let add x y = x + y
2 |>add<| 2
let (++) x y = x + y
let lostNumbers = [4;8;15;16;23;42]
let cards = [3; 7; 12]

let mix = lostNumbers @ cards
List.concat [[1;2;3];[4;5]]

let lostNumbers = [13;666]
[1; "Гарри Поттер"; 16.5]
[]
let fcards = 1 :: cards
[[1; 2]; [3; 4]] @ [[5; 6]]
[[1; 2]; [3; 4]]