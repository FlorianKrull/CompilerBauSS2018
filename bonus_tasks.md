# Bonus Tasks

You are not obliged to implement them, but doing so may shift your grade.

## General

- Setup continuous integration (CI) so new revisions are automatically built and tested.
  See [GitLab CI](https://about.gitlab.com/features/gitlab-ci-cd/) or [Travis CI](https://travis-ci.org/).

- Parallelise the following stages of your compilation pipeline:

  - parsing of multiple source files
  - running semantic checks
  - Control-Flow Graph generation per function
  - TAC generation per function
  - ASM code generation per function
  - ...

  This is about making your compiler even faster by utilising multiple threads.
  Most tasks done by the compiler work on files / functions independently and are therefore easy to parallelise.

  I recommend maintaining a thread pool with about as many threads as their are CPU threads and a work queue from which the threads draw their tasks.
  You should also add a way to override the thread pool size, either through an environment variable or a commandline parameter (`-j`).

- Allow AST transformations to be done by *plugins*.
  Define an API which every plugin has to follow.
  Each plugin is compiled as standalone against this API, the result is a shared library which is loaded dynamically at runtime (using `dlopen` and alike).
  Which plugins the compiler should use can then be specified via commandline parameters, eg:

      $ ./mCc --plugin plugins/remove_unnecessary_parenthesis.so --plugin plugins/simplify_unary_minus_on_literal.so -o example01 example01.mC

## Assignment 1

- Implement a visitor mechanism which houses the logic of traversing the AST.
  Use this visitor for all tasks involving AST traversal.

- Investigate the tracing / debugging options provided by flex / bison.
  Provide a way to access these debugging information.

- Implement the following AST transformation:

      single_expr(unary_op("-"), expression(single_expr(literal(α))))  ⟹  single_expr(literal(- α))

## Assignment 2

- Implement some form of trace output for the type checking process.

- Provide an executable to dump symbol tables in a human readable format.

- Use static single assignment (SSA).

## Assignment 3

- Implement a visitor mechanism for your CFGs.

- Implement the generation of the call graph.

- Perform register allocation.

## Assignment 4

- Implement some form of error recovery.
  Do not simply terminate the parsing process upon encountering an error.
  Continue at a sensible point in the token stream and accumulate errors.
