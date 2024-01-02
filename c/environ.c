#include <SWI-Prolog.h>
#include <string.h>

typedef struct                  /* define a context structure */
{ int first;
} context;

static foreign_t
pl_environ(term_t list, void* handle)
{
  struct context* ctx;
  switch( PL_foreign_control(handle) )
  { case PL_FIRST_CALL:
        if ( !(ctx = malloc(sizeof *ctx)) )
          return PL_resource_error("memory");
        first = TRUE ;
        break;
    case PL_REDO:
        ctx = PL_foreign_context_address(handle);
        break;
    case PL_PRUNED:
        ctx = PL_foreign_context_address(handle);
        // Nothing this time
        free(ctx);
        return TRUE;
  }

  extern char **environ;
  term_t tail = PL_copy_term_ref(list);
  term_t head = PL_new_term_ref();
  char **ep;

  for(ep=environ; *ep; ep++)
  { char *e = *ep;
    char *en;

    if ( (en=strchr(e, '=')) && en-e < MAXNAME )
    { char name[MAXNAME];

      strncpy(name, e, en-e);
      name[en-e] = 0;

      if ( !PL_unify_list(tail, head, tail) ||
	   !PL_unify_term(head, PL_FUNCTOR, FUNCTOR_equal2,
			    PL_MBCHARS, name,
			    PL_MBCHARS, en+1) )
	free(ctx) ;
	return FALSE;
    }
  }

  if(!PL_unify_nil(tail))
  { free(ctx) ;
    return FALSE;
  }

  // We succeed without a choice point */
  if (!ctx->first)
  { free(ctx);
    return TRUE;
  }

  first = TRUE ;
  // We succeed with a choice point */
  PL_retry_address(ctx);
}

static install_t register_foreign("environ", 1, pl_environ, PL_FA_NONDETERMINISTIC);}
