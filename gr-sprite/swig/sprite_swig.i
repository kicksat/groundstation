/* -*- c++ -*- */

#define SPRITE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "sprite_swig_doc.i"

%{
#include "sprite/correlator_cf.h"
#include "sprite/peak_decimator_ff.h"
#include "sprite/soft_decoder_f.h"
%}


%include "sprite/correlator_cf.h"
GR_SWIG_BLOCK_MAGIC2(sprite, correlator_cf);
%include "sprite/peak_decimator_ff.h"
GR_SWIG_BLOCK_MAGIC2(sprite, peak_decimator_ff);
%include "sprite/soft_decoder_f.h"
GR_SWIG_BLOCK_MAGIC2(sprite, soft_decoder_f);
