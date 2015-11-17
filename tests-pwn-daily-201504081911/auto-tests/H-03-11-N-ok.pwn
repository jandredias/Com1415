#pwn() = 0 {
  #x;
  repeat (x = 0; x < 30; x = x + 1)
    if (x % 2 == 0)
       repeat (; x < 15; x = x + 1)
          x!!
}