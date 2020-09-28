/**
 * @file lfs_brickwall_pd.c
 * @author Lynn, Felix, Stephan
 * Audiocommunication Group, Technical University Berlin <br>
 * A distortion object for pure data <br>
 * <br>
 * @brief Audio Object for a distortion <br>
 * <br>
 * RTAP Distortion 9000 allows to select one of 16 distortion types and choose how much distortion will be applied to the signal. It is recommended to use multiple instances with bandpass filters to get the most out of this external.
 * <br>
 */

#include "m_pd.h"
#include "lfs_dist_dsp.h"
#include "lfs_dist_dsp_test.h"


static t_class *rtap_distortion9000_tilde_class;

/**
 * @struct rtap_distortion9000_tilde
 * @brief The Pure Data struct of the rtap_distortion9000l~ object. <br>
 * @var lfs_dist_dsp::x_obj Necessary for every signal object in Pure Data <br>
 * @var lfs_dist_dsp::f Also necessary for signal objects, float dummy dataspace <br>
 * for converting a float to signal if no signal is connected (CLASS_MAINSIGNALIN) <br>
 * @var lfs_dist_dsp::gain The gain object for the actual signal processing <br>
 * @var lfs_dist_dsp::x_out A signal outlet for the adjusted signal
 * level of the incoming signal
 */
typedef struct rtap_distortion9000_tilde
{
    t_object  x_obj;
    t_sample f;
    lfs_dist_dsp *dist_dsp;
    t_outlet *x_out;
} rtap_distortion9000_tilde;

/**
 * @related rtap_distortion9000_tilde_perform
 * @brief Calculates the volume adjusted output vector<br>
 * @param w A pointer to the object, input and output vectors. <br>
 * For more information please refer to the Pure Data Docs <br>
 * The function calls the lfs_brickwall_perform method. <br>
 * @return A pointer to the signal chain right behind the lfs_brickwall_tilde object. <br>
 */
t_int *rtap_distortion9000_tilde_perform(t_int *w)
{
    rtap_distortion9000_tilde *x = (rtap_distortion9000_tilde *)(w[1]);
    t_sample  *in = (t_sample *)(w[2]);
    t_sample  *out =  (t_sample *)(w[3]);
    int n =  (int)(w[4]);

    lfs_dist_dsp_perform(x->dist_dsp, in, out, n);

    /* return a pointer to the dataspace for the next dsp-object */
    return (w+5);
}

/**
 * @related rtap_distortion9000_tilde_dsp
 * @brief Adds lfs_brickwall_tilde_perform to the signal chain. <br>
 * @param x A pointer the lfs_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void rtap_distortion9000_tilde_dsp(rtap_distortion9000_tilde *x, t_signal **sp)
{
    dsp_add(rtap_distortion9000_tilde_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

/**
 * @related rtap_distortion9000_tilde_free
 * @brief Frees our object. <br>
 * @param x A pointer the lfs_brickwall_tilde object <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void rtap_distortion9000_tilde_free(rtap_distortion9000_tilde *x)
{
    outlet_free(x->x_out);
    lfs_dist_dsp_free(x->dist_dsp);
}

/**
 * @related rtap_distortion9000_tilde_new
 * @brief Creates a new lfs_brickwall_tilde object.<br>
 * @param f Sets the initial gain value. <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void *rtap_distortion9000_tilde_new(t_floatarg dryWet, t_floatarg distortionMod, t_floatarg saturation)
{
    rtap_distortion9000_tilde *x = (rtap_distortion9000_tilde *)pd_new(rtap_distortion9000_tilde_class);
    
    //The main inlet is created automatically
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->dist_dsp = lfs_dist_dsp_new();
    x->dist_dsp->dryWet = dryWet;
    x->dist_dsp->distortionMod = distortionMod;
    x->dist_dsp->saturation = saturation;

    return (void *)x;
}

/**
 * @related rtap_distortion9000_tilde_setdryWet
 * @brief Sets the drywet adjustment parameter. <br>
 * @param x A pointer the lfs_dist_dsp_tilde object <br>
 * @param level Sets the level parameter <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void rtap_distortion9000_tilde_setdryWet(rtap_distortion9000_tilde *x, float dryWet)
{
	lfs_dist_dsp_setDryWet(x->dist_dsp, dryWet);
}

/**
 * @related rtap_distortion9000_tilde_setdistortionMod
 * @brief Sets the distortion type parameter. <br>
 * @param x A pointer the lfs_dist_dsp_tilde object <br>
 * @param distortionMod Sets the distortion type parameter <br>
 */
void rtap_distortion9000_tilde_setdistortionMod(rtap_distortion9000_tilde *x, float distortionMod)
{
	lfs_dist_dsp_setDistortionMod(x->dist_dsp, distortionMod);
}

/**
 * @related rtap_distortion9000_tilde_setSaturation
 * @brief Sets the saturation parameter. <br>
 * @param x A pointer the lfs_dist_dsp_tilde object <br>
 * @param saturation Sets the saturation parameter <br>
 */
void rtap_distortion9000_tilde_setSaturation(rtap_distortion9000_tilde *x, float saturation)
{
    lfs_dist_dsp_setSaturation(x->dist_dsp, saturation);
}

void rtap_distortion9000_tilde_integrationTest(rtap_distortion9000_tilde *x)
{
    lfs_dist_dsp_test *test = lfs_dist_dsp_test_new(x->dist_dsp, 32, 64);
    lfs_dist_dsp_test_integration(test);
    lfs_dist_dsp_test_free(test);
}


/**
 * @related rtap_distortion9000_tilde_setup
 * @brief Setup of lfs_dist_dsp_tilde <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void rtap_distortion9000_tilde_setup(void)
{
    rtap_distortion9000_tilde_class = class_new(gensym("rtap_distortion9000~"), (t_newmethod)rtap_distortion9000_tilde_new, (t_method)rtap_distortion9000_tilde_free, sizeof(rtap_distortion9000_tilde), CLASS_DEFAULT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
    
    class_addmethod(rtap_distortion9000_tilde_class, (t_method)rtap_distortion9000_tilde_dsp, gensym("dsp"), 0);
    class_addmethod(rtap_distortion9000_tilde_class, (t_method)rtap_distortion9000_tilde_setdryWet, gensym("drywet"), A_DEFFLOAT, 0);
    class_addmethod(rtap_distortion9000_tilde_class, (t_method)rtap_distortion9000_tilde_setdistortionMod, gensym("distortionMod"), A_DEFFLOAT, 0);
    class_addmethod(rtap_distortion9000_tilde_class, (t_method)rtap_distortion9000_tilde_setSaturation, gensym("saturation"), A_DEFFLOAT, 0);
    class_addmethod(rtap_distortion9000_tilde_class, (t_method)rtap_distortion9000_tilde_integrationTest, gensym("integrationtest"), 0);
    CLASS_MAINSIGNALIN(rtap_distortion9000_tilde_class, rtap_distortion9000_tilde, f);
}
