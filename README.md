#Quine–McCluskey Algorithm 

Simple implementation of [Quine–McCluskey algorithm](http://en.wikipedia.org/wiki/Quine%E2%80%93McCluskey_algorithm) in C language. It uses arrays and lists to provide a good insight (hopefully) of their utility in an actual algorithm. Although due to the use of the above data structures, it is pretty inefficient both in terms of time and space.

##Input format

The input format is given in a single string, containing all the minterms of the function we want to minimize. Example:

	>> 0,2,5,8,9,13,15

The input above is equivalent to the function below:

	F = Σm(0,2,5,8,9,13,15)

## TODO
- Show **prime implicant chart** followed by *essentials* prime implicants
- Complete *minimizing* boolean function
- Support for **don't care** conditions
- Input format as boolean function

##Contact

If you have any question or suggestion don't hesitate to mail me at [kkanelli@hotmail.com](mailto:kkanelli@hotmail.com) 

