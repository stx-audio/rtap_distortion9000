/*
 * HOWTO write an External for Pure data
 * (c) 2001-2006 IOhannes m zmölnig zmoelnig[AT]iem.at
 *
 * this is the source-code for the second example in the HOWTO
 * it creates an object that increments and outputs a stx_counter
 * whenever it gets banged.
 *
 * for legal issues please see the file LICENSE.txt
 */



/**
 * include the interface to Pd 
 */
#include "m_pd.h"

/**
 * define a new "class" 
 */
static t_class *stx_counter_class;



/**
 * this is the dataspace of our new object
 * the first (mandatory) "t_object"
 * and a variable that holds the current stx_counter value
 */
typedef struct _stx_counter {
  t_object  x_obj;
  t_int i_count; /* a variable to store something inside */
    t_float step;
    t_int i_down, i_up;
    t_outlet *f_out, *b_out;
} t_stx_counter;


/**
 * this method is called whenever a "bang" is sent to the object
 * a reference to the class-dataspace is given as argument
 * this enables us to do something with the data (e.g. increment the stx_counter)
 */
void stx_counter_bang(t_stx_counter *x)
{
  /*
   * convert the current stx_counter value to floating-point to output it later
   */
  t_float f = x->i_count;
    
    /* store stepsize */
    int step = x->step;
    /* increase the counter by stepsize */
    x->i_count += step;
    
    if (x->i_down - x->i_up) {
        if ( (step > 0) && (x->i_count > x->i_up) ) {
            x->i_count = x->i_down;
            outlet_bang(x->b_out);
        } else if (x->i_count > x->i_down) {
            outlet_bang(x->b_out);
        }
    }
  /* send the old stx_counter-value to the 1st outlet of the object */
  outlet_float(x->x_obj.ob_outlet, f);
}

void stx_counter_reset(t_stx_counter *x)
{
    x->i_count = x->i_down;
}

void stx_counter_set(t_stx_counter *x, t_floatarg f)
{
    x->i_count = f;
}

void stx_counter_bound(t_stx_counter *x, t_floatarg f1, t_floatarg f2)
{
    x->i_down = (f1<f2)?f1:f2;
    x->i_up   = (f1>f2)?f1:f2;
}

/* the new task for the constructor here is to initialize a variable value and to create an outlet for the object. instead of void the argument is now an floatarg and than it sets the variable in the struct to this given argument.    */
/**
 * this is the "constructor" of the class
 * we have one argument of type floating-point (as specified below in the stx_counter_setup() routine)
 */
void *stx_counter_new(t_symbol *s, int argc, t_atom *argv)
{
  t_stx_counter *x = (t_stx_counter *)pd_new(stx_counter_class);

  /* set the stx_counter value to the given argument */
    t_float f1 = 0, f2 = 0;
    
    x->step = 1; /* stepwidth defaults to 1 if no 3rd argument is passed */
    switch(argc){ /* how many arguments are passed? */
        default:
        case 3:
            x->step = atom_getfloat(argv+2); /* stepwidth */
        case 2:
            f2 = atom_getfloat(argv+1); /* upper boundary */
        case 1:
            f1 = atom_getfloat(argv); /* lower boundary */
        case 0:
            break;
    }
    if (argc < 2) f2 = f1;
    x->i_down = (f1<f2)?f1:f2;
    x->i_up = (f1>f2)?f1:f2;
    
    x->i_count = x->i_down;

    /* create a 1st ACTIVE inlet (first is always active) */
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("list"), gensym("bound"));
    
    /* create a 2nd PASSIVE inlet */
    floatinlet_new(&x->x_obj, &x->step);
    
    /* create a new outlet for floating-point values */
    x->f_out = outlet_new(&x->x_obj, &s_float);
    
    /* create a new outlet for bang triggers */
    x->b_out = outlet_new(&x->x_obj, &s_bang);
    
    /* the returned pointers of the outlets have to be stored to use them later in the outlet routines */

  return (void *)x;
}


/**
 * define the function-space of the class. an initial value can be set by the user - A_DEFFLOAT tells the object that it needs one argument ot the type t_floatarg
 */
void stx_counter_setup(void) {
  stx_counter_class = class_new(gensym("stx_counter"),
			    (t_newmethod)stx_counter_new,
			    0, 
			    sizeof(t_stx_counter),
			    CLASS_DEFAULT,
			    A_GIMME, 0); /* the object takes a dynamic number of arguments */

  /* call a function when object gets banged */
  class_addbang(stx_counter_class, stx_counter_bang);
    
    class_addmethod(stx_counter_class,
                    (t_method)stx_counter_reset,
                    gensym("reset"),
                    0);
    
    class_addmethod(stx_counter_class,
                    (t_method)stx_counter_set,
                    gensym("set"),
                    A_DEFFLOAT,
                    0);
    
    class_addmethod(stx_counter_class,
                    (t_method)stx_counter_bound,
                    gensym("bound"),
                    A_DEFFLOAT,
                    A_DEFFLOAT,
                    0);
    
    class_sethelpsymbol(stx_counter_class, gensym("help-stx-counter"));
    
}
