#include <SWI-Prolog.h>
#include <string.h>

typedef struct  
{ char** ep;
} context;

static functor_t FUNCTOR_equal2;
#define MAXNAME 512

static foreign_t
pl_environ(term_t result, void* handle)
{
  extern char **environ;

  context* ctx;
  switch(PL_foreign_control(handle))
  { case PL_FIRST_CALL:
        if (!(ctx = malloc(sizeof *ctx)))
          return PL_resource_error("memory");
        ctx->ep = environ;
        break;
    case PL_REDO:
        ctx = PL_foreign_context_address(handle);
        break;
    case PL_PRUNED:
        ctx = PL_foreign_context_address(handle);
        free(ctx);
        return TRUE;
  }

  if(*(ctx->ep) == NULL)
  {  /* No environment variables defined, fail (1) */
    free(ctx);
    return(FALSE);
  }

  char *e = *(ctx->ep);
  char *en;
  if((en=strchr(e, '=')) && en-e < MAXNAME)
  { char name[MAXNAME];
    strncpy(name, e, en-e);
    name[en-e] = 0;

    if(!PL_unify_term(result, PL_FUNCTOR, FUNCTOR_equal2,
      PL_MBCHARS, name, PL_MBCHARS, en+1))
    { free(ctx);
      return FALSE;
    }
  }

  /* This check is somehow redundant with (1), but avoids a choice 
     point when no variables are left. */
  ctx->ep++;
  if(*(ctx->ep) == NULL)
  { free(ctx);
    return(TRUE);
  }

  /* We succeed with a choice point */
  PL_retry_address(ctx);
}

install_t
install_environ()
{ FUNCTOR_equal2 = PL_new_functor(PL_new_atom("="), 2);
  PL_register_foreign("environ", 1, pl_environ, PL_FA_NONDETERMINISTIC);
}

