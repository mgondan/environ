:- module(test_environ,
          [ test_environ/0
          ]).
:- use_module(library(plunit)).
:- use_module(library(debug)).

% Load the library from our pack that needs to be tested
:- use_module(library(environ)).

test_environ :-
    run_tests([ environ
              ]).

:- begin_tests(environ).

test(basic) :-
    environ(List),
    assertion(is_list(List)).

:- end_tests(environ).
