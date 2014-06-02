/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "correlator_cf_impl.h"
#include <gnuradio/gr_complex.h>
#include <gnuradio/fft/fft.h>
#include <complex>
#include <cmath>

using namespace std;

namespace gr {
  namespace sprite {

    correlator_cf::sptr
    correlator_cf::make(int prn_id)
    {
      return gnuradio::get_initial_sptr
        (new correlator_cf_impl(prn_id));
    }

    /*
     * The private constructor
     */
    correlator_cf_impl::correlator_cf_impl(int prn_id)
      : gr::sync_block("correlator_cf",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
      set_history(SPRITE_PRN_LENGTH);

      generate_prn(prn_id);
  
      cc430_modulator(m_prn, m_template);
      for (int k = 0; k < SPRITE_PRN_LENGTH; k++)
      {
        m_template[k] = conj(m_template[k]);
      }
      
      m_fft = new fft::fft_complex(SPRITE_PRN_LENGTH, true, 1);
      m_fft_buffer_in = m_fft->get_inbuf();
      m_fft_buffer_out = m_fft->get_outbuf();
    }

    /*
     * Our virtual destructor.
     */
    correlator_cf_impl::~correlator_cf_impl()
    {
    }


    void correlator_cf_impl::cc430_modulator(int* prnBits, gr_complex* baseBand)
    {
      float* diffs = m_buffer_real1;
      float* iBB = m_buffer_real2;
      float* qBB = m_buffer_real3;
      
      //Differentially encode with +/-1 values
      diffs[0] = -2*prnBits[0] + 1;
      for (int k = 1; k < SPRITE_PRN_LENGTH; k++)
      {
        char diff = prnBits[k]-prnBits[k-1];
        if(diff == 0)
        {
          diffs[k] = 1;
        }
        else
        {
          diffs[k] = -1;
        }
      }
      
      //Initialize with offset between I and Q
      iBB[0] = 1;
      qBB[0] = diffs[0];
      qBB[1] = diffs[0];
      
      for(int k = 1; k < SPRITE_PRN_LENGTH-2; k+=2)
      {
        iBB[k] = diffs[k]*iBB[k-1];
        iBB[k+1] = iBB[k];
      }
      iBB[SPRITE_PRN_LENGTH-1] = diffs[SPRITE_PRN_LENGTH-1]*iBB[SPRITE_PRN_LENGTH-2];
      
      for(int k = 2; k < SPRITE_PRN_LENGTH; k+=2)
      {
        qBB[k] = diffs[k]*qBB[k-1];
        qBB[k+1] = qBB[k];
      }
      
      for(int k = 0; k < SPRITE_PRN_LENGTH; k++)
      {
        baseBand[k] = iBB[k]*cos(M_PI/2*k) + 1i*qBB[k]*sin(M_PI/2*k);
      }
    }

    void correlator_cf_impl::generate_prn(int prn_id)
    {
      if(prn_id == -2)
      { 
        //Deep copy M-sequence
        for (int k = 0; k < M_SEQUENCE_LENGTH; k++)
        {
          m_prn[k] = mseq1[k];
        }
      }
      else if(prn_id == -1)
      { 
        //Deep copy M-sequence
        for (int k = 0; k < M_SEQUENCE_LENGTH; k++)
        {
          m_prn[k] = mseq2[k];
        }
      }
      else //if(prn_id >= 0 && prn_id < M_SEQUENCE_LENGTH)
      { 
        //Generate Gold Codes by xor'ing 2 M-sequences in different phases
        for (int k = 0; k < M_SEQUENCE_LENGTH-prn_id; k++)
        {
          m_prn[k] = mseq1[k] ^ mseq2[k+prn_id];
        }
        for (int k = M_SEQUENCE_LENGTH-prn_id; k < M_SEQUENCE_LENGTH; k++)
        {
          m_prn[k] = mseq1[k] ^ mseq2[k-M_SEQUENCE_LENGTH+prn_id];
        }
      }

      m_prn[SPRITE_PRN_LENGTH-1] = 0; //To pad out the last byte, add a zero to the end
    }

    int
    correlator_cf_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        float *out = (float *) output_items[0];

        // Do <+signal processing+>
        for(int k = 0; k < noutput_items; ++k) {
          
          //Pointwise multiply by baseband template and copy to fft input
          for (int j = 0; j < SPRITE_PRN_LENGTH; ++j)
          {
            m_fft_buffer_in[j] = m_template[j]*in[j+k];
          }
          
          //Take FFT
          m_fft->execute();
          
          //Find largest value in FFT
          float mag2 = real(m_fft_buffer_out[0]*conj(m_fft_buffer_out[0]));
          float max = mag2;
          for (int j = 1; j < SPRITE_PRN_LENGTH; ++j)
          {
            mag2 = real(m_fft_buffer_out[j]*conj(m_fft_buffer_out[j]));
            if (mag2 > max)
            {
              max = mag2;
            }
          }
          
          out[k] = sqrt(max);
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace sprite */
} /* namespace gr */

