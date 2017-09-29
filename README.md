# Expr

This repository is a simple implementation of a calculator that uses Reverse polish notation (RPN)
in order to solve input expressions in standard infix notation.

The four basic math operations are supported and it works only with integers. If you are
looking for some practical stuff to start on compiler/interpreter design, maybe this
code can help you.

## How can I clone this repo?

The repository has some sub-modules, the easiest way of cloning it is as follows:

```
you@Somewhere:~/src# git clone https://github.com/rafael-santiago/expr expr --recursive
```

Done!

## How can I build it?

Until now I successfully built  it on ``Linux``, ``FreeBSD``, ``NetBSD``, ``OpenBSD``, ``MINIX``,
``Solaris`` and also ``Windows``. This project uses ``ANSI C``, so virtually you will be able to
easily build this code in any decent platform.

For my own projects I always use my own [build system](https://github.com/rafael-santiago/hefesto).
After following the steps to make ``hefesto`` work on your system all you should do is:

```
you@Somewhere:~/src# cd expr/src
you@Somewhere:~/src/expr/src# hefesto
```

After calling ``hefesto`` the codes will be compiled and some tests will run. The binary will be
within the directory ``../bin``.

Anyway, if you are not interested in running tests and all this stuff, you can build the software
by hand. This is a pretty minimalist project and you can easily compile it in just one simple single
command line:

```
you@Somewhere:~/src/expr/src# cc memory.c stack.c expr.c rpn.c main.c -oexpr
```

## How can I use it?

Being within the sub-directory ``src``, supposing that you have built the program with my
build system, just call ``../bin/expr``.

A prompt will be shown and all you should do is to type your infix expression and so
confirm it hit ENTER to get the result back.

To exit the program type CTRL+c and also confirm it with an ENTER.
