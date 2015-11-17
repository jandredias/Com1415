import #argc()
import $argv(#n)
import #atoi($s)

#factorial(#n) = 1 {
  if (n > 0) factorial = n * factorial(n-1);
  else factorial = 1;
}

#pwn() = 0 {
  #f = 1;
  "Teste para a função factorial"!!
  if (argc() == 2) f = atoi(argv(1));
  factorial(f);
  f! "! = "! factorial(f)!!
}
