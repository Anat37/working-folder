module File1

let elem a l =
    let rec elem' l n =
        match l with 
        | l::ls when (l=a) -> n
        | l::ls -> elem' ls (n+1) 
        | _ -> -1
    elem' l 0


let rec set = function 
    | l::ls when (elem l ls = -1) -> set ls
    | l::ls -> l::(set ls)
    | [] -> []


let rec frec l = 
    let rec frec' l a n =
        match l with
        | l::ls when (a=l) -> frec' ls a (n+1)
        | l::ls -> frec' ls a n
        | _ -> n
    match set l with 
    | x::xs -> (x, frec' l x 0)::(frec xs)
    | [] -> []

let rec zipWith f l1 l2=
    match (l1, l2) with 
    | (x::xs, y::ys) -> (f x y)::zipWith f xs ys
    | _ -> [] 


