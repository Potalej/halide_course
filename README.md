# halide_course

Para rodar o docker, use:
```
  docker build -t local:halidecpp .
  docker run -it -v .:/src local:halidecpp
```

Para acessar a paǵina com este código dentro do docker, rode:
```
  cd /src
```

E aí para compilar é:
```
  cmake .
  make
```