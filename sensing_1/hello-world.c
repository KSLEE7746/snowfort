/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * $Id: hello-world.c,v 1.1 2006/10/02 21:46:46 adamdunkels Exp $
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "i2c.h"
#include "spi.h"
#include <stdio.h> /* For printf() */
#define debug
#define DUR (0)

  unsigned rv, xx0, xx1, yy0, yy1, zz0, zz1;
  int x, y, z;
  float xx, yy, zz;
  
void init();
void set_measure();
void measure();
void strange_measure();
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();
	int i;
// configuration -- bundle of single byte writes
#ifdef debug
printf("configuring\n");
#endif
		init();
// Major loop for reading
do{

// testing i2c channel by reading whoami register, standard output should be 229
#ifdef debug
printf("accessing WHOAMI register...\n");
#endif
i2c_enable();
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x00);
		i2c_stop();
		i2c_start();
		i2c_write(0xA7);
		rv = i2c_read(0);
		i2c_stop();
		
i2c_disable();

		if(rv == 229)
			printf("i2c working correctly, this sensor is 229\n");
		else{
			printf("ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!\n");
			for(i=0; i<2500000; i++){
				_NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
			}
		}

#ifdef debug
printf("Setting measurement mode before each measurement\n");
#endif		
			//enable measurement before each reading;
			set_measure();
			
			// */
			
			
// get acceleration on x, y, z axis
//		measure();
		strange_measure();

//append the 2 bytes		
		x = 0;
		x |= xx1;
#ifdef debug
printf("debuging...\nx: %d\tx1: %d\n", x, xx1);
#endif
		x = (x<<8) | xx0;
		xx=x*0.0078;
		
		y = 0;
		y |= yy1;
#ifdef debug
printf("y: %d\ty1: %d\n", y, yy1);
#endif
		y = (y<<8) | yy0;
		yy=y*0.0078;
		
		z = 0;
		z |= zz1;
#ifdef debug
printf("z: %d\tz1: %d\n", z, zz1);
#endif
		z = (z<<8) | zz0;
		zz=z*0.0078;

		printf("X: %d %d \nY: %d %d \nZ: %d %d \n",  xx0, xx1, yy0, yy1, zz0, zz1);
		printf("X value: %d\tY value: %d\tZ value: %d\n\n\n", x, y, z);

		for(i =1 ; i<=25000; i++){
			_NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
		}

}while(1);
  PROCESS_END();
}


void init(){
	i2c_enable();

//0x40 - thresh-tap
//0x0
//0x0
//0x0 - three offsets
//0x7f duration
//0x30 - latency
//0x7f - window
//0x2 thresh_act
//0x1 thresh_inact
//0xff time_inact
//0xff act_inact_ctl
//0x05 thresh_ff
//0x14 time_ff
//0x7 tap_axes
//0x0 act_tap_st
//0x0a bw_rate
//0x08 power_ctl
//0x0 int_enable
//0x01 data format

		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x1D);
		i2c_write(0x40);		//0x40 - thresh-tap
		i2c_stop();
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x1E);
		i2c_write(0x00);		//0x0
		i2c_stop();

		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x1F);
		i2c_write(0x00);		//0x0
		i2c_stop();

		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x20);
		i2c_write(0x00);		//0x0 - three offsets
		i2c_stop();

		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x21);
		i2c_write(0x7f);		//0x7f duration
		i2c_stop();		
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x22);
		i2c_write(0x30);		//0x30 - latency
		i2c_stop();
		
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x23);
		i2c_write(0x7f);		//0x7f - window
		i2c_stop();

		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x24);
		i2c_write(0x02);		//0x2 thresh_act
		i2c_stop();
		
				
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x25);
		i2c_write(0x01);		//0x1 thresh_inact
		i2c_stop();
				
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x26);
		i2c_write(0xff);		//0xff time_inact
		i2c_stop();
				
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x27);
		i2c_write(0xff);		//0xff act_inact_ctl
		i2c_stop();		
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x28);
		i2c_write(0x05);		//0x05 thresh_ff
		i2c_stop();		
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x29);
		i2c_write(0x14);		//0x14 time_ff
		i2c_stop();		
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x2A);
		i2c_write(0x07);		//0x7 tap_axes
		i2c_stop();		
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x2B);
		i2c_write(0x00);		//0x0 act_tap_st
		i2c_stop();
		
				
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x2C);
		i2c_write(0x0a);		//0x0a bw_rate
		i2c_stop();		
// */

/*
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x2D);
		i2c_write(0x00);		//set to standby mode first as recommanded
		i2c_stop();	
		// */
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x2D);
		i2c_write(0x08);		//0x08 power_ctl
		i2c_stop();		
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x2E);
		i2c_write(0x00);		//0x0 int_enable
		i2c_stop();
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x31);
		i2c_write(0x01);		//0x1 data_format
		i2c_stop();

// */
/*		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x1D);
		i2c_write(0x40);//0x40 - thresh-tap
		i2c_write(0x00);//0x0
		i2c_write(0x00);//0x0
		i2c_write(0x00);//0x0 - three offsets
		i2c_write(0x7f);//0x7f duration
		i2c_write(0x30);//0x30 - latency
		i2c_write(0x7f);//0x7f - window
		i2c_write(0x02);//0x2 thresh_act
		i2c_write(0x01);//0x1 thresh_inact
		i2c_write(0xff);//0xff time_inact
		i2c_write(0xff);//0xff act_inact_ctl
		i2c_write(0x05);//0x05 thresh_ff
		i2c_write(0x14);//0x14 time_ff
		i2c_write(0x07);//0x7 tap_axes
		i2c_write(0x00);//0x0 act_tap_st
		i2c_write(0x0a);//0x0a bw_rate
		i2c_write(0x08);//0x08 power_ctl
		i2c_write(0x00);//0x0 int_enable
		i2c_stop();
// */		
i2c_disable();
}

void set_measure(){
	int i;
	i2c_enable();
	/*
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x2D);
		i2c_write(0x00);		//set to standby mode first as recomanded
		i2c_stop();	
	// */	
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x2D);
		i2c_write(0x08);		//0x08 power_ctl
		i2c_stop();	
	
	i2c_disable();
	
#ifdef debug
printf("wait for measure.\n");
#endif	
		for(i =1 ; i<=DUR; i++){
			_NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
		}
}

void measure(){ // standard data accessing according to the datasheet
i2c_enable();

		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x32);
		i2c_stop();
		i2c_start();
		i2c_write(0xA7);
		xx0=i2c_read(1);
		xx1=i2c_read(1);
		yy0=i2c_read(1);
		yy1=i2c_read(1);
		zz0=i2c_read(1);
		zz1=i2c_read(0);
		i2c_stop();
		
i2c_disable();
}

void strange_measure(){ // non_standard reading. accessing each axis separately
i2c_enable();

		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x32);
		i2c_stop();
		i2c_start();
		i2c_write(0xA7);
		xx0=i2c_read(0);
		i2c_stop();
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x33);
		i2c_stop();
		i2c_start();
		i2c_write(0xA7);
		xx1=i2c_read(0);
		i2c_stop();
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x34);
		i2c_stop();
		i2c_start();
		i2c_write(0xA7);
		yy0=i2c_read(0);
		i2c_stop();
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x35);
		i2c_stop();
		i2c_start();
		i2c_write(0xA7);
		yy1=i2c_read(0);
		i2c_stop();
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x36);
		i2c_stop();
		i2c_start();
		i2c_write(0xA7);
		zz0=i2c_read(0);
		i2c_stop();
		
		i2c_start();
		i2c_write(0xA6);
		i2c_write(0x37);
		i2c_stop();
		i2c_start();
		i2c_write(0xA7);
		zz1=i2c_read(0);
		i2c_stop();		

		
i2c_disable();
}