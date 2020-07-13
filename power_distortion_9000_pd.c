/**
 * @file power_distortion_9000_pd.c
 * @author Thomas Resch <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * University of Applied Sciences Nordwestschweiz (FHNW), Music-Academy, Research and Development <br>
 * A gain object for pure data <br>
 * <br>
 * @brief A Pure Data gain object for adjusting the volume<br>
 * <br>
 * power_distortion_9000~ allows for adjusting the level<br>
 * of any incoming audio signal. <br>
 * <br>
 */

#include "m_pd.h"
#include "power_distortion_9000.h"

static t_class *power_distortion_9000_tilde_class;

/**
 * @struct power_distortion_9000_tilde
 * @brief The Pure Data struct of the power_distortion_9000~ object. <br>
 * @var power_distortion_9000_tilde::x_obj Necessary for every signal object in Pure Data <br>
 * @var power_distortion_9000_tilde::f Also necessary for signal objects, float dummy dataspace <br>
 * for converting a float to signal if no signal is connected (CLASS_MAINSIGNALIN) <br>
 * @var power_distortion_9000_tilde::gain The gain object for the actual signal processing <br>
 * @var power_distortion_9000_tilde::x_out A signal outlet for the adjusted signal
 * level of the incoming signal
 */

typedef struct power_distortion_9000_tilde
{
    t_object  x_obj;
    t_sample f;
    power_distortion_9000 *gain;
    t_outlet *x_out;
} power_distortion_9000_tilde;

/**
 * @related power_distortion_9000_tilde
 * @brief Calculates the volume adjusted output vector<br>
 * @param w A pointer to the object, input and output vectors. <br>
 * For more information please refer to the Pure Data Docs <br>
 * The function calls the power_distortion_9000_perform method. <br>
 * @return A pointer to the signal chain right behind the power_distortion_9000_tilde object. <br>
 */

t_int *power_distortion_9000_tilde_perform(t_int *w)
{
    power_distortion_9000_tilde *x = (power_distortion_9000_tilde *)(w[1]);
    t_sample  *in = (t_sample *)(w[2]);
    t_sample  *out =  (t_sample *)(w[3]);
    int n =  (int)(w[4]);

    power_distortion_9000_perform(x->gain, in, out, n);

    /* return a pointer to the dataspace for the next dsp-object */
    return (w+5);
}

/**
 * @related power_distortion_9000_tilde
 * @brief Adds power_distortion_9000_tilde_perform to the signal chain. <br>
 * @param x A pointer the power_distortion_9000_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void power_distortion_9000_tilde_dsp(power_distortion_9000_tilde *x, t_signal **sp)
{
    dsp_add(power_distortion_9000_tilde_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

/**
 * @related power_distortion_9000_tilde
 * @brief Frees our object. <br>
 * @param x A pointer the power_distortion_9000_tilde object <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void power_distortion_9000_tilde_free(power_distortion_9000_tilde *x)
{
    outlet_free(x->x_out);
    power_distortion_9000_free(x->gain);
}

/**
 * @related power_distortion_9000_tilde
 * @brief Creates a new power_distortion_9000_tilde object.<br>
 * @param f Sets the initial gain value. <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void *power_distortion_9000_tilde_new(t_floatarg f)
{
    power_distortion_9000_tilde *x = (power_distortion_9000_tilde *)pd_new(power_distortion_9000_tilde_class);
    
    //The main inlet is created automatically
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->gain = power_distortion_9000_new();
    x->gain->level = f;

    return (void *)x;
}

/**
 * @related power_distortion_9000_tilde
 * @brief Sets the gain adjustment parameter. <br>
 * @param x A pointer the power_distortion_9000_tilde object <br>
 * @param level Sets the level parameter <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void power_distortion_9000_tilde_setLevel(power_distortion_9000_tilde *x, float level)
{
    x->gain->level = level;
}

/**
 * @related power_distortion_9000_tilde
 * @brief Setup of power_distortion_9000_tilde <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void power_distortion_9000_tilde_setup(void)
{
      power_distortion_9000_tilde_class = class_new(gensym("power_distortion_9000~"),
            (t_newmethod)power_distortion_9000_tilde_new,
            (t_method)power_distortion_9000_tilde_free,
        sizeof(power_distortion_9000_tilde),
            CLASS_DEFAULT,
            A_DEFFLOAT, 0);

      class_addmethod(power_distortion_9000_tilde_class, (t_method)power_distortion_9000_tilde_dsp, gensym("dsp"), 0);

      // this adds the gain message to our object
      class_addmethod(power_distortion_9000_tilde_class, (t_method)power_distortion_9000_tilde_setLevel, gensym("gain"), A_DEFFLOAT,0);

      CLASS_MAINSIGNALIN(power_distortion_9000_tilde_class, power_distortion_9000_tilde, f);
}
