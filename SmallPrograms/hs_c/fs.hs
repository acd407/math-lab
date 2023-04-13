-- ghc -dynamic -shared fs.hs -o mylib.so -fforce-recomp -O2 -fPIC -lHSrts-ghc9.0.2
module Fs where

demo :: IO ()
demo = print (fastsort [1, 9, 5, 6, 7, 4, 8, 2, 3])

fastsort :: (Ord a) => [a] -> [a]
fastsort [] = []
fastsort (e:l) = fastsort [x|x <- l, x < e] ++ [e] ++ fastsort [x|x <- l, x > e]

foreign export ccall demo :: IO ()