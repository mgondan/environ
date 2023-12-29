:- module(environ,
  [ environ/1			% -Environment
  ]).

:- use_foreign_library(foreign(environ)).

:- begin_tests(environ).

test(one) :-
  true.

:- end_tests(environ).
