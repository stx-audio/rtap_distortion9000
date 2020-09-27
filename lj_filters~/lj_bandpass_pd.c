/**
 * @file lj_bandpass_pd.c
 * @author Thomas Resch <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * University of Applied Sciences Nordwestschweiz (FHNW), Music-Academy, Research and Development <br>
 * A gain object for pure data <br>
 * <br>
 * @brief A Pure Data gain object for adjusting the volume<br>
 * <br>
 * lj_bandpass~ allows for adjusting the level<br>
 * of any incoming audio signal. <br>
 * <br>
 */

#include "m_pd.h"
#include "lj_bandpass.h"

static t_class *lj_bandpass_tilde_class;

/**
 * @struct lj_bandpass_tilde
 * @brief The Pure Data struct of the lj_bandpass~ object. <br>
 * @var lj_bandpass_tilde::x_obj Necessary for every signal object in Pure Data <br>
 * @var lj_bandpass_tilde::f Also necessary for signal objects, float dummy dataspace <br>
 * for converting a float to signal if no signal is connected (CLASS_MAINSIGNALIN) <br>
 * @var lj_bandpass_tilde::gain The gain object for the actual signal processing <br>
 * @var lj_bandpass_tilde::x_out A signal outlet for the adjusted signal
 * level of the incoming signal
 */

typedef struct lj_bandpass_tilde
{
    t_object  x_obj;
    t_sample f;
    LJ_bandpass *bandpass;
    t_outlet *x_out;
} LJ_bandpass_tilde;

/**
 * @related LJ_bandpass_tilde
 * @brief Calculates the volume adjusted output vector<br>
 * @param w A pointer to the object, input and output vectors. <br>
 * For more information please refer to the Pure Data Docs <br>
 * The function calls the lj_bandpass_perform method. <br>
 * @return A pointer to the signal chain right behind the lj_bandpass_tilde object. <br>
 */

t_int *lj_bandpass_tilde_perform(t_int *w)
{
    LJ_bandpass_tilde *x = (LJ_bandpass_tilde *)(w[1]);
    t_sample  *in = (t_sample *)(w[2]);
    t_sample  *out =  (t_sample *)(w[3]);
    int n =  (int)(w[4]);

    lj_bandpass_perform(x->bandpass, in, out, n);

    /* return a pointer to the dataspace for the next dsp-object */
    return (w+5);
}

/**
 * @related lj_bandpass_tilde
 * @brief Adds lj_bandpass_tilde_perform to the signal chain. <br>
 * @param x A pointer the lj_bandpass_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void lj_bandpass_tilde_dsp(LJ_bandpass_tilde *x, t_signal **sp)
{
    dsp_add(lj_bandpass_tilde_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

/**
 * @related lj_bandpass_tilde
 * @brief Frees our object. <br>
 * @param x A pointer the lj_bandpass_tilde object <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void lj_bandpass_tilde_free(LJ_bandpass_tilde *x)
{
    outlet_free(x->x_out);
    lj_bandpass_free(x->bandpass);
}

/**
 * @related lj_bandpass_tilde
 * @brief Creates a new lj_bandpass_tilde object.<br>
 * @param highpass_cutoff Sets the highpass filter cutoff frequency . <br>
 * @param lowpass_cutoff Sets the lowpass filter cutoff frequency. <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void *lj_bandpass_tilde_new(t_floatarg highpass_cutoff, t_floatarg lowpass_cutoff)
{
    LJ_bandpass_tilde *x = (LJ_bandpass_tilde *)pd_new(lj_bandpass_tilde_class);
    
    //The main inlet is created automatically
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->bandpass = lj_bandpass_new();
    
    return (void *)x;
}

/**
 * @related lj_bandpass_tilde
 * @brief Sets the highpass filter cutoff frequency  <br>
 * @param x A pointer the lj_bandpass_tilde object <br>
 * @param hp_fc Sets the highpass filter cutoff frequency<br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void lj_bandpass_tilde_setHighCutoff(LJ_bandpass_tilde *x, float hp_fc)
{
     lj_bandpass_setHighCutoff(x->bandpass, hp_fc);
}

/**
 **
 * @related lj_bandpass_tilde
 * @brief Sets the lowpass filter cutoff frequency  <br>
 * @param x A pointer the lj_bandpass_tilde object <br>
 * @param hp_fc Sets the lowpass filter cutoff frequency<br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void lj_bandpass_tilde_setLowCutoff(LJ_bandpass_tilde *x, float lp_fc)
{
    lj_bandpass_setLowCutoff(x->bandpass, lp_fc);
}

/**
 * @related lj_bandpass_tilde
 * @brief Setup of lj_bandpass_tilde <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void lj_bandpass_tilde_setup(void)
{
      lj_bandpass_tilde_class = class_new(gensym("lj_bandpass~"),
            (t_newmethod)lj_bandpass_tilde_new,
            (t_method)lj_bandpass_tilde_free,
        sizeof(LJ_bandpass_tilde),
            CLASS_DEFAULT,
            A_DEFFLOAT, 0);

      class_addmethod(lj_bandpass_tilde_class, (t_method)lj_bandpass_tilde_dsp, gensym("dsp"), 0);

     /**adding messages for highpass and lowpass cutoffs*/
      class_addmethod(lj_bandpass_tilde_class, (t_method)lj_bandpass_tilde_setHighCutoff, gensym("HighCutoff"), A_DEFFLOAT,0);
    
    class_addmethod(lj_bandpass_tilde_class, (t_method)lj_bandpass_tilde_setLowCutoff, gensym("LowCutoff"), A_DEFFLOAT,0);

      CLASS_MAINSIGNALIN(lj_bandpass_tilde_class, LJ_bandpass_tilde, f);
}
