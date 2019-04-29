// -----------------------------------------------------------
// Software:         GW302 - McHale V6 Fixed Chamber Round Baler
// C Source File:    display.c
// Description:      Graphic Wizard Display Routines
// -----------------------------------------------------------


// Include Standard files
#include "Board.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"


// functions

// -----------------------------------------------------------
// Function Name       : setPixel
// -----------------------------------------------------------
void setPixel( unsigned char *buff, unsigned char x, unsigned char y, unsigned int method )
{
	unsigned char n;
	unsigned short int offset;

	offset = (x / 8) + (y * COLUMNS);
	if( offset < GRAPHICS_SIZE )
	{
		n = (unsigned char)0x80 >> (x % 8);

		if( method == CLEAR )
		{
			buff[offset] &= ~n;
		}
		else
		{
			buff[offset] |= n;
		}
	}
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : circlePoints
// -----------------------------------------------------------
void circlePoints( unsigned char *buff, unsigned char cx, unsigned char cy,
  unsigned char x, unsigned char y, unsigned int method )
{

	if( x == 0 )
	{
		setPixel( buff, cx, cy + y, method );
		setPixel( buff, cx, cy - y, method );
		setPixel( buff, cx + y, cy, method );
		setPixel( buff, cx - y, cy, method );
	}
	else if( x == y )
	{
		setPixel( buff, cx + x, cy + y, method );
		setPixel( buff, cx - x, cy + y, method );
		setPixel( buff, cx + x, cy - y, method );
		setPixel( buff, cx - x, cy - y, method );
	}
	else if( x < y )
	{
		setPixel( buff, cx + x, cy + y, method );
		setPixel( buff, cx - x, cy + y, method );
		setPixel( buff, cx + x, cy - y, method );
		setPixel( buff, cx - x, cy - y, method );
		setPixel( buff, cx + y, cy + x, method );
		setPixel( buff, cx - y, cy + x, method );
		setPixel( buff, cx + y, cy - x, method );
		setPixel( buff, cx - y, cy - x, method );
	}
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : circleMidpoint
// -----------------------------------------------------------
void circleMidpoint( unsigned char *buff, unsigned char xCenter, unsigned char yCenter,
  unsigned char radius, unsigned int method )
{
	unsigned char x = 0;
	unsigned char y = radius;
	signed int p = (5 - (radius*4)) / 4;

	circlePoints( buff, xCenter, yCenter, x, y, method );
	while( x < y )
	{
		x++;
		if( p < 0 )
		{
			p += ((2 * x) + 1);
		}
		else
		{
			y--;
			p += ((2*(x-y)) + 1);
		}
		circlePoints( buff, xCenter, yCenter, x, y, method );
	}
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : greatest
// -----------------------------------------------------------
unsigned char greatest( unsigned char *a, unsigned char *b )
{
	unsigned char c;

	if ( *a < *b )
	{
		c = *b;
		*b = *a;
		*a = c;
	}

	return( *a - *b );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : draw_line
// -----------------------------------------------------------
void draw_line( unsigned char *buff, unsigned char xp1, unsigned char yp1,
  unsigned char xp2, unsigned char yp2, unsigned int method )
{
	unsigned char n, x, y, dx, dy, dir;
	unsigned short int offset;

	if ( xp1 > 159 )
		xp1 = 159;
	if ( xp2 > 159 )
		xp2 = 159;
	if ( yp1 > 127 )
		yp1 = 127;
	if ( yp2 > 127 )
		yp2 = 127;

	if ( ((xp2 >= xp1) && (yp2 >= yp1)) || ((xp2 < xp1) && (yp2 < yp1)) )
		dir = 0;
	else
		dir = 1;

	dx = greatest( &xp2, &xp1 );
	dy = greatest( &yp2, &yp1 );

	if( dir )
	{
		x = xp2;
		for ( y = 0; y <= dy; y++ )
		{
			offset = (x / 8) + ((y + yp1) * COLUMNS);
			n = (unsigned char)0x80 >> (x % 8);
			while ( ((y * dx) >= ((xp2 - x) * dy)) && (x >= xp1) )
			{

				if ( method == CLEAR )
					buff[offset] &= ~n;
				else
					buff[offset] |= n;
				if ( !(n <<= 1) )
				{
					n = 0x01;
					offset--;
				}
				if( x )
					x--;
				else
					break;
			}
			x++;
		}
	}
	else
	{
		x = xp1;
		for ( y = 0; y <= dy; y++ )
		{
			offset = (x / 8) + ((y + yp1) * COLUMNS);
			n = (unsigned char)0x01 << (7 - (x % 8));
			while ( ((y * dx) >= ((x - xp1) * dy)) && (x <= xp2) )
			{
				if ( method == CLEAR )
					buff[offset] &= ~n;
				else
					buff[offset] |= n;
				if ( !(n >>= 1) )
				{
					n = 0x80;
					offset++;
				}
				x++;
			}
			x--;
		}
	}

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : draw_box
// -----------------------------------------------------------
void draw_box( unsigned char *buff, unsigned char xp1, unsigned char yp1,
  unsigned char xp2, unsigned char yp2, unsigned int method )
{
	/* GREY, CLEAR, FULL, INV or EMPTY(default), */

	unsigned char start_byte, end_byte, right_shift, left_shift, dx, bytes;
	unsigned char n, c, x, y, cc, nn, mask;
	unsigned short int offset, point;

	if ( xp1 > 159 )
		xp1 = 159;
	if ( xp2 > 159 )
		xp2 = 159;
	if ( yp1 > 127 )
		yp1 = 127;
	if ( yp2 > 127 )
		yp2 = 127;

	dx = greatest( &xp2, &xp1 );
	greatest( &yp2, &yp1 );

	if ( !dx )
	{
		draw_line( buff, xp1, yp1, xp1, yp2, method );
		return;
	}
	else if ( dx == 1)
	{
		draw_line( buff, xp1, yp1, xp1, yp2, method );
		draw_line( buff, xp2, yp1, xp2, yp2, method );
		return;
	}

	start_byte = xp1 / 8;
	right_shift = xp1 % 8;
	end_byte = xp2 / 8;
	left_shift = 7-(xp2 % 8);
	bytes = end_byte - start_byte;

	c = (unsigned char)0xff >> right_shift;
	n = (unsigned char)0xff << left_shift;

	switch ( method )
	{
	case GREY:
		for ( y = yp1; y <= yp2; y++ )
		{
			mask = (y & 1) ? 0xaa : 0x55;
			cc = c & mask;
			nn = n & mask;

			offset = start_byte + (y * COLUMNS);
			if ( !bytes )
				buff[offset++] &= ((cc & nn) | ~(c & n));
			else
			{
				buff[offset++] &= (cc | ~c);
				for ( x = start_byte+1; x < end_byte; x++ )
					buff[offset++] &= mask;
				buff[offset] &= (nn | ~n);
			}
		}
		break;

	case CLEAR:
		for ( y = yp1; y <= yp2; y++ )
		{
			offset = start_byte + (y * COLUMNS);
			if ( !bytes )
				buff[offset++] &= ~(c & n);
			else
			{
				buff[offset++] &= ~c;
				for ( x = start_byte+1; x < end_byte; x++ )
					buff[offset++] = 0x00;
				buff[offset] &= ~n;
			}
		}
		break;

	case FULL:
		//case FULLCLP:
			for ( y = yp1; y <= yp2; y++ )
			{
				offset = start_byte + (y * COLUMNS);
				if ( !bytes )
					buff[offset++] |= (c & n);
				else
				{
					buff[offset++] |= c;
					for ( x = start_byte+1; x < end_byte; x++ )
						buff[offset++] |= 0xff;
					buff[offset] |= n;
				}
			}
			break;

	case INV:
	case INVCLP:
		for( y = yp1; y <= yp2; y++ )
		{
			offset = start_byte + (y * COLUMNS);
			if( !bytes )
			{
				//buff[offset] &= ~(c & n);
				//buff[offset] |= ~c ;
				//buff[offset++] |= ~n ;
				buff[offset] ^= (c & n);
			}
			else
			{
				//cc = ~buff[offset];
				//cc &= c;
				//buff[offset] &= ~c;
				//buff[offset] |= cc;
				buff[offset] ^= c;

				offset++;

				for( x = start_byte + 1; x < end_byte; x++ )
				{
					buff[offset] ^= 0xff;
					offset++;
				}

				//nn = ~buff[offset];
				//nn &= n;
				//buff[offset] &= ~n;
				//buff[offset] |= nn;
				buff[offset] ^= n;

				offset++;
			}
		}
		break;

		//case EMPTYCLP:
	default: /* EMPTY */
		offset = start_byte + (yp1 * COLUMNS);
		point =  start_byte + (yp2 * COLUMNS);

		if ( !bytes )
		{
			c = c & n;
			buff[offset++] |= c;
			buff[point++] |= c;
		}
		else
		{
			buff[offset++] |= c;
			buff[point++] |= c;

			for ( x = start_byte+1; x < end_byte; x++ )
			{
				buff[offset++] |= 0xff;
				buff[point++] |= 0xff;
			}
			buff[offset] |= n;
			buff[point] |= n;
		}

		c = (unsigned char)0x80 >> right_shift;
		n = (unsigned char)0x01 << left_shift;

		for ( y = yp1+1; y < yp2; y++ )
		{
			offset = start_byte + (y * COLUMNS);
			point =  end_byte + (y * COLUMNS);
			buff[offset] |= c;
			buff[point] |= n;
		}
		break;
	}

	if( ( method == INVCLP )/* || ( method == EMPTYCLP ) || ( method == FULLCLP ) */)
	{ /* clip the corners off */
		draw_line( adm, xp1, yp1, xp1, yp1, CLEAR );
		draw_line( adm, xp2, yp1, xp2, yp1, CLEAR );
		draw_line( adm, xp1, yp2, xp1, yp2, CLEAR );
		draw_line( adm, xp2, yp2, xp2, yp2, CLEAR );
	}
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : clip_box
// -----------------------------------------------------------
void clip_box( unsigned char xp1, unsigned char yp1, unsigned char xp2, unsigned char yp2 )
{
	draw_line( adm, xp1, yp1, xp1, yp1, CLEAR );
	draw_line( adm, xp2, yp1, xp2, yp1, CLEAR );
	draw_line( adm, xp1, yp2, xp1, yp2, CLEAR );
	draw_line( adm, xp2, yp2, xp2, yp2, CLEAR );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : write_bitmap
// Output Parameters   : ptd - a buffer to write raster scan for display driver
//                       pts - uncompressed bitmap array
//                       pts[0] = bit width of bit map
//                       pts[1] = bit height of bit map
//                       xpos,ypos - x,y location on display, top left = 0,0
//                       method - AND, OR, INV
// -----------------------------------------------------------
unsigned char write_bitmap( unsigned char *ptd, const unsigned char *pts,
  unsigned char xpos, unsigned char ypos, unsigned int method )
{
	unsigned short int j;
	unsigned char *lptd, *lpts;
	unsigned char imL, imR, c, d, width, x, y, n;

	if( method & INV )
	{
		imL = (unsigned char)0xff;
		imR = (unsigned char)0xff << (8-(pts[0] % 8));
	}
	else
	{
		imL = 0;
		imR = 0;
	}

	c = xpos % 8; // xpos adjust, right shift bitmap
	d = 8 - c;
	width = (pts[0] + 7) / 8;


	lpts = (unsigned char *)&pts[2];
	lptd = &buffer[0];
	for( y = 0; y < pts[1]; y++ )
	{
		*lptd++ = ((*lpts ^ imL) >> c);
		for( x = 1; x < width; x++ )
		{
			*lptd = ((*lpts ^ imL) << d);
			if( x == (width - 1) )
			{
				*lptd++ |= ((*(++lpts) ^ imR) >> c);
			}
			else
			{
				*lptd++ |= ((*(++lpts) ^ imL) >> c);
			}
		}
		*lptd++ = ((*lpts++ ^ imR) << d);
	}

	width++;

	x = (xpos / 8);
	lpts = &buffer[0];
	for( n = 0; n < pts[1]; n++ )
	{
		j = x + ((ypos + n) * COLUMNS);
		lptd = &ptd[j];
		for( c=0; (c < width) && (j < GRAPHICS_SIZE); c++ )
		{
			if( (method & OR) == OR )
			{
				*lptd++ |= *lpts++;
			}
			else if ( (method & XOR) == XOR )
			{
				*lptd++ ^= *lpts++;
			}
			else  //  default = AND
					{
				*lptd++ = *lpts++;
					}
			j++;
		}
	}

	return( pts[0] );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : display_f1_text
// -----------------------------------------------------------
unsigned char display_f1_text( const unsigned char *data, unsigned char max,
  unsigned char co, unsigned char ro, unsigned char prog )
{
	static unsigned char pos, dstring[32];

	struct fn1 const *font;
	unsigned short int i;
	unsigned char *pt, loc, chr, wid, lco, coo;

	font = fnt1[pFac.pf.Lang];

	if( prog & EDIT )
	{
		if( editing )
		{
			if( key & (SW_NEW | SW_REP) )
			{
				key &= ~(SW_NEW | SW_REP); // mask off status bits
				if( key == SW_OR )
				{
					switch( editing )
					{
					case EDIT_UPPER:
						editing = EDIT_LOWER;
						break;

					case EDIT_LOWER:
						editing = EDIT_NUMERIC;
						break;

					case EDIT_NUMERIC:
						editing = EDIT_UPPER;
						break;
					}
				}
				else if( key == SW_IR )
				{
					editing = 0;
					strcpy( (char *)data, dstring );
					// if factor is in pFac address space then save to flash
					if( ((unsigned char *)data >= &(pFac.ch[0])) && ((unsigned char *)data < (unsigned char *)&(pFac.ch[sizeof(struct PreservedFactors)])) )
					{
						savecal = 1;
					}

					if( prog & EDSCAPE )
					{
						escape = 3;
					}
				}
				else if( key == SW_CR )
				{
					if( pos < (max - 1) )
					{
						pos++;
					}
				}
				else if( key == SW_CL )
				{
					if( pos )
					{
						pos--;
					}
				}
				else if( key == SW_IL )
				{
					editing = 0;
					if( prog & EDSCAPE )
					{
						escape = 2;
					}
				}
				else if( (key == SW_CU) || (key == SW_CD) )
				{
					chr = dstring[pos];
					i = 0;
					// find match in upper case data
					while( UCkey[i] && (UCkey[i] != chr) )
					{
						i++;
					}
					//  match
					if( UCkey[i] )
					{
						switch( editing )
						{
						case EDIT_UPPER:
							pt = (unsigned char *)&UCkey[0];
							loc = i;
							break;

						case EDIT_LOWER:
							pt = (unsigned char *)&LCkey[0];
							loc = i;
							break;

						case EDIT_NUMERIC:
							pt = (unsigned char *)&NCkey[0];
							loc = 0;
							break;
						}
					}
					else // no match
					{
						i = 0;
						// find match in lower case data
						while( LCkey[i] && (LCkey[i] != chr) )
						{
							i++;
						}
						if( LCkey[i] )
						{
							switch( editing )
							{
							case EDIT_UPPER:
								pt = (unsigned char *)&UCkey[0];
								loc = i;
								break;

							case EDIT_LOWER:
								pt = (unsigned char *)&LCkey[0];
								loc = i;
								break;

							case EDIT_NUMERIC:
								pt = (unsigned char *)&NCkey[0];
								loc = 0;
								break;
							}
						}
						else // no match
						{
							i = 0;
							// find match in numeric data
							while( NCkey[i] && (NCkey[i] != chr) )
							{
								i++;
							}
							if( NCkey[i] )
							{
								switch( editing )
								{
								case EDIT_UPPER:
									pt = (unsigned char *)&UCkey[0];
									loc = 0;
									break;

								case EDIT_LOWER:
									pt = (unsigned char *)&LCkey[0];
									loc = 0;
									break;

								case EDIT_NUMERIC:
									pt = (unsigned char *)&NCkey[0];
									loc = i;
									break;
								}
							}
							else // no match
							{
								switch( editing )
								{
								case EDIT_UPPER:
									pt = (unsigned char *)&UCkey[0];
									loc = 0;
									break;

								case EDIT_LOWER:
									pt = (unsigned char *)&LCkey[0];
									loc = 0;
									break;

								case EDIT_NUMERIC:
									pt = (unsigned char *)&NCkey[0];
									loc = 0;
									break;
								}
							}
						}
					}

					if( key == SW_CU )
					{
						// set next character or first
						loc++;
						if( *(pt + loc) )
						{
							dstring[pos] = *(pt + loc);
						}
						else
						{
							dstring[pos] = *pt;
						}

					}
					else // key == SW_CD
					{
						// set previous character or last
						if( loc )
						{
							loc--;
							dstring[pos] = *(pt + loc);
						}
						else
						{
							while( *pt )
							{
								pt++;
							}
							pt--;
							dstring[pos] = *pt;
						}
					}

				}
			}
		}
		else if( (prog & EDSCAPE) || (key == (SW_NEW | SW_CR)) )
		{
			editing = EDIT_UPPER;
			pos = 0;
			looptime = 0;
			strcpy( dstring, data );
			i = max;
			dstring[i--] = 0x00;
			while( (dstring[i] == 0x00) && i )
			{
				dstring[i--] = ' ';
			}
		}
	}

	wid = 0;
	if( (prog & EDIT) && editing )
	{
		for( i=0; (chr = dstring[i]) && (i < max); i++ )
		{
			wid += (font->f1[chr][0] + font->spc);
		}
	}
	else
	{
		for( i=0; (chr = *(data+i)) && (i < max); i++ )
		{
			wid += (font->f1[chr][0] + font->spc);
		}
	}
	wid = wid - font->spc - 1;

	if( prog & CENTER )
	{
		co = co - (wid / 2);
	}
	wid += co;

	if( (prog & EDIT) && editing )
	{
		draw_box( adm, co-1, ro-1, wid+1, ro+8, CLEAR );
	}
	else
	{
		draw_box( adm, co-1, ro-1, wid+1, ro+7, CLEAR );
	}

	if( prog & INV )
	{
		draw_box( adm, co-1, ro-1, wid+1, ro+8, EMPTY );
	}

	coo = co;

	if( (prog & EDIT) && editing )
	{
		for( i=0; (chr = dstring[i]) && (i < max); i++ )
		{
			lco = co;
			co += (write_bitmap( adm, font->f1[chr], co, ro, OR ) + font->spc);
			if( (i == pos) && !half_second )
			{
				draw_box( adm, lco-1, ro-1, co-1, ro+8, INV );
			}
		}
	}
	else
	{
		for( i=0; (chr = *(data+i)) && (i < max); i++ )
		{
			co += (write_bitmap( adm, font->f1[chr], co, ro, OR ) + font->spc);
		}
	}

	if( prog & GREY )
	{
		draw_box( adm, coo, ro, wid, ro+((prog & EDIT) ? 8 : 6), GREY );
	}

	if( prog & INV )
	{
		draw_box( adm, coo, ro, wid, ro+((prog & EDIT) ? 8 : 6), INV );
	}

	if( prog & UNDERLINED )
	{
		draw_line( adm, coo, ro+8, co-1, ro+8, OR );
	}

	return( co );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : display_number
// -----------------------------------------------------------
unsigned char display_number( float data, float min, float max, float con,
        unsigned char dp, unsigned char xc, unsigned char yc, unsigned char width, unsigned char just,
        struct NumericFont const *font, void *factor, unsigned char ftype )
{
	static float sdata;
	static char len;

	unsigned char ldp, ed, x1, x2, ch, wi, hi, *pt;
	unsigned char tstring[16];

	// get edit box size
	hi = font->height + yc - 1;
	if( (just & dn_JUST) == RIGHT )
	{
		x1 = xc - width;
		x2 = xc;
	}
	else
	{
		x1 = xc;
		x2 = xc + width;
	}

	// clear space for number (including invert box), leave xc at start
	draw_box( adm, x1, yc-1, x2, hi+1, CLEAR );
	//draw_box( adm, x1, yc, x2, hi, CLEAR );

	data *= con;

	ldp = dp;
	if( ldp )
	{
		if( data >= 1000.0 )
		{
			ldp = 0;
		}
		else if( data >= 100.0 )
		{
			if( ldp > 1 )
			{
				ldp = 1;
			}
		}
		else if( data >= 10.0 )
		{
			if( ldp > 2 )
			{
				ldp = 2;
			}
		}
	}

	if( (ftype & PINC) && (just & EDIT) ) // simple increment
			{
		if( editing )
		{
			if( key & (SW_NEW | SW_REP) )
			{
				key &= ~(SW_NEW | SW_REP); // mask off status bits
				if( key == SW_CR )
				{
					if( sdata < max )
					{
						sdata++;
					}
				}
				else if( key == SW_CL )
				{
					if( sdata > min )
					{
						sdata--;
					}
				}
				else if( key == SW_IL )
				{
					editing = 0xfe; // cancel editing without save - but still copy number to tstring
				}
				else if( key == SW_IR )
				{
					editing = 0xff; // cancel editing with save - but still copy number to tstring
				}
			}
		}
		else if( (just & EDSCAPE) || (key == (SW_NEW | SW_IR)) )
		{
			sdata = data;
			editing = 1;
			looptime = 0;
		}
			}

	if( !(ftype & PINC) && (just & EDIT) && editing )  // digit program
	{
		// editing string so no sprintf needed
	}
	else
	{
		if( (just & dn_SIGN) == NSIGN )
		{
			pt = "%+.*f";
		}
		else
		{
			pt = "%.*f";
		}
		len = sprintf( tstring, pt, ldp, ((ftype & PINC) && (just & EDIT) && editing) ? sdata : data );

		// check for fit
		ch = font->gap - 1; // character spacing
		wi = font->bitmap[0][0] + ch; // character width + space
		ed = ((width + ch) / wi); // maximum length of string in characters

		if( len > ed )
		{
			len = ed;
			tstring[ed] = 0x00;
			while( ed )
			{
				ed--;
				tstring[ed] = '9';
			}
		}
	}

	if( !(ftype & PINC) && (just & EDIT) ) // digit program
	{
		if( editing )
		{
			if( key & (SW_NEW | SW_REP) )
			{
				key &= ~(SW_NEW | SW_REP); // mask off status bits
				pt = &number[editing - 1];
				/*if( ((window == screen132) && (key == SW_IL)) || ((window != screen132) && (key == SW_CU)) )*/
				if( key == SW_IL )
				{
					if( *pt == '+' )
					{
						*pt = '-';
					}
					else if( *pt == '-' )
					{
						*pt = '+';
					}
					else if( *pt == '.' )
					{
						// increase multiplier if dp position < field width
						if( editing < (len - 1) )
						{
							ch = *(pt + 1);
							*(pt + 1) = *pt;
							*pt = ch;
							editing++;
						}
					}
					else
					{
						(*pt)++;
						if( *pt > '9' )
						{
							*pt = '0';
						}
					}
				}
				//else if( ((window != screen132)&& (key == SW_CD)) || ((window == screen132) && (key == SW_OL)) )
				else if( key == SW_CD )
				{
					if( *pt == '+' )
					{
						*pt = '-';
					}
					else if( *pt == '-' )
					{
						*pt = '+';
					}
					else if( *pt == '.' )
					{
						// decrease multiplier if dp position >(field width - set precision)
						if( editing > (len - dp) )
						{
							editing--;
							ch = *(pt - 1);
							*(pt - 1) = *pt;
							*pt = ch;
						}
					}
					else
					{
						(*pt)--;
						if( *pt < '0' )
						{
							*pt = '9';
						}
					}
				}
				else if( key == SW_CL )
				{
					if( editing > 1 )
					{
						editing--;
					}
				}
				else if( key == SW_IL )
				{
					editing = 0xfe; // cancel editing without save - but still copy number to tstring
				}
				else if( key == SW_CR )
				{
					if( number[editing] != 0x00 ) // end of string
					{
						editing++;
					}
				}
				else if( key == SW_IR )
				{
					editing = 0xff; // cancel editing with save - but still copy number to tstring
				}
			}
		}
		else if( (just & EDSCAPE) || (key == (SW_NEW | SW_IR)) )
		{
			editing = 1;
			looptime = 0;
			strcpy( number, tstring );
			// insert leading zeros
			ch = strlen( number );
			wi = width / (font->bitmap[0][0] + font->gap - 1);
			len = wi;
			if( ch < wi )
			{
				while( ch )
				{
					number[wi--] = number[ch--];
				}
				number[wi--] = number[ch--];
				while( wi )
				{
					number[wi--] = '0';
				}
				number[wi] = '0';
			}
		}

		if( editing )
		{
			strcpy( tstring, number );
		}
	}

	if( just & EDIT ) // digit & inc program
	{
		if( editing == 0xfe )
		{
			editing = 0;
			if( just & EDSCAPE )
			{
				escape = 2;
			}
		}
		else if( editing == 0xff )
		{
			editing = 0;
			if( just & EDSCAPE )
			{
				escape = 3;
			}

			if( factor != NULL )
			{
				switch( ftype & PTYPE )
				{
				case PCHAR:
					*(unsigned char *)factor = (ftype & PINC) ? (unsigned char)sdata : (unsigned char)atoi( number );
					if( *(unsigned char *)factor > (unsigned char)max )
					{
						*(unsigned char *)factor = (unsigned char)max;
					}
					else if( (unsigned char)min && (*(unsigned char *)factor < (unsigned char)min) )
					{
						*(unsigned char *)factor = (unsigned char)min;
					}
					break;

				case PSHORT:
					*(unsigned short int *)factor = (ftype & PINC) ? (unsigned short int)sdata : (unsigned short int)atoi( number );
					if( *(unsigned short int *)factor > (unsigned short int)max )
					{
						*(unsigned short int *)factor = (unsigned short int)max;
					}
					else if( (unsigned short int)min && (*(unsigned short int *)factor < (unsigned short int)min) )
					{
						*(unsigned short int *)factor = (unsigned short int)min;
					}
					break;

				case PLONG:
					*(unsigned long int *)factor = (ftype & PINC) ? (unsigned long int)sdata : (unsigned long int)atol( number );
					if( *(unsigned long int *)factor > (unsigned long int)max )
					{
						*(unsigned long int *)factor = (unsigned long int)max;
					}
					else if( (unsigned long int)min && (*(unsigned long int *)factor < (unsigned long int)min) )
					{
						*(unsigned long int *)factor = (unsigned long int)min;
					}
					break;

				case PFLOAT:
					*(float *)factor = ((ftype & PINC) ? sdata : atoff( number )) / con;
					if( *(float *)factor > max )
					{
						*(float *)factor = max;
					}
					else if( *(float *)factor < min )
					{
						*(float *)factor = min;
					}
					break;
				}

				// if factor is in pFac address space then save to flash
				if( ((unsigned char *)factor >= &(pFac.ch[0])) && ((unsigned char *)factor < (unsigned char *)&(pFac.ch[sizeof(struct PreservedFactors)])) )
				{
					savecal = 1;
				}
				valuechanged = 1;
			}

		}
	}

	wi = 0;
	pt = &tstring[0];
	while( *pt )
	{
		ch = *pt;
		switch( ch )
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			ch = ch - '0';
			break;

		case '.':
			ch = 10;
			break;

		case '+':
			ch = 11;
			break;

		case '-':
			ch = 12;
			break;

		default: // unknown
			ch = 13;
			break;
		}

		// save value + 1, so '0' is not 0x00
		*pt = ch + 1;

		// width of character
		wi += font->bitmap[ch][0];

		pt++;

		// last character needs no space
		if( *pt )
		{
			wi += font->gap - 1;
		}
	}

	if( (just & dn_JUST) == RIGHT )
	{
		xc -= wi;
	}

	pt = &tstring[0];
	ed = 1;
	while( *pt )
	{
		ch = *pt - 1;

		write_bitmap( adm, font->bitmap[ch], xc, yc, OR );
		wi = xc;
		xc += font->bitmap[ch][0];

		if( !(ftype & PINC) && (just & EDIT) && editing && (editing == ed) )
		{
			if( !key && !half_second )
			{
				draw_box( adm, wi-1, yc-1, xc, hi+1, INV ); // invert character
				// draw_box( adm, x1, yc-1, x2, hi+1, INV ); // invert whole string
				//draw_box( adm, wi-1, yc-1, xc, hi+1, CLEAR ); // blank character
			}
			//draw_box( adm, wi, hi+1, xc-1, hi+2, ch ? EMPTY : CLEAR ); // underscore
		}

		// check to see if last character, if not add gap
		pt++;
		if( *pt )
		{
			xc += font->gap - 1;
		}
		ed++;
	}

	if( (ftype & PINC) && (just & EDIT) && editing ) // simple increment & editing, flash all
	{
		if( !half_second )
		{
			draw_box( adm, x1, yc-1, x2, hi+1, INV ); // flash all
		}
	}

	//if( (just & EDIT) && !editing )
	//  {
	//  draw_box( adm, x1, yc-1, x2, hi+1, INV );
	//  }

	return( xc );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : draw_frame
// -----------------------------------------------------------
void draw_frame( unsigned char *pt )
{
	unsigned int i;

	// clear global display buffer
	for( i = 0; i < GRAPHICS_SIZE; i++ )
	{
		*(pt+i) = 0x00;
	}
	// draw empty frame
	draw_line( pt, 0,   0, 159,   0, OR );
	draw_line( pt, 0,   0,   0, 127, OR );
	draw_line( pt, 0, 127, 159, 127, OR );
	draw_line( pt, 159, 0, 159, 127, OR );
}    
// -----------------------------------------------------------


#ifndef WINDOWS

// --- Hardware specific functions below --- 

// -----------------------------------------------------------
// Function Name       : SendToDisplay
// -----------------------------------------------------------
void SendToDisplay( unsigned char *pbuffer, int count, char mode )
{
	// wait until SPI PDC ready
	while( !(AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXBUFE) )
	{
	}

	// set command or data mode
	AT91F_SPI_CfgMode( AT91C_BASE_SPI0,
			AT91C_SPI_MSTR |
			AT91C_SPI_PS_FIXED | (AT91C_SPI_PCS & ((unsigned int)mode << 16)) ); // CS1 == command/CS0 = data

	// send data
	AT91F_PDC_SetTx( AT91C_BASE_PDC_SPI0, pbuffer, count );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : WriteDisplay
// -----------------------------------------------------------
void WriteDisplay( unsigned char *pts )
{
	unsigned int j, pageoffset;
	unsigned char *lptd;
	unsigned char page, c, mask;
	unsigned char command[4];
	unsigned char map[160];

	// set up command Column 0, Page 0
	command[0] = 0x00; // set Column LSN (0x00 | '0000CCCC')
	command[1] = 0x10; // set Column MSN (0x10 | '0000CCCC') 240 columns max
	command[2] = 0x60; // set Page LSN (0x60 | '0000PPPP')
	// start on page 16 for PM5718 240x128
	command[3] = 0x71; // set Page MSN (0x70 | '00000PPP') 80 pages max

	// send command
	SendToDisplay( command, 4, DM_COMMAND );

	// write to 64 pages (128 lines)
	for( page = 0; page < 64; page++ )
	{
		// wait until SPI PDC ready
		while( !(AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXBUFE) )
		{
		}

		// set up command - set column 40
		command[0] = 0x08; // set Column LSN (0x00 | '0000CCCC')
		command[1] = 0x12; // set Column MSN (0x10 | '0000CCCC') 240 columns max
		// send command
		SendToDisplay( command, 2, DM_COMMAND );
		// wait until SPI PDC ready

		while( !(AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXBUFE) )
		{
		}

		// remap for Ultra UC1611
		lptd = &map[0];
		pageoffset = page * ((160 / 8) * 2); // page * bytes per line * lines per page
		for( c = 1; c <= 160; c++ ) // columns
		{
			mask = 0x01 << ((c - 1) % 8);

			// first pixel on first line
			j = pageoffset + ((160 - c) / 8); // result
			if( pts[j] & mask )
			{
				*lptd = (DISPLAY_DEFAULT_MASK & 0x0f);
			}
			else
			{
				*lptd = 0x00;
			}

			// next pixel from line below
			// j = pageoffset + ((320 - c) / 8); // result
			j += (160 / 8); // + bytes per line
			if( pts[j] & mask )
			{
				*lptd |= (DISPLAY_DEFAULT_MASK & 0xf0);
			}
			lptd++;
		}
		// send data
		SendToDisplay( map, 160, DM_DATA );
	}

	// wait until SPI PDC ready
	while( !(AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXBUFE) )
	{
	}

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : SPI0Setup
// -----------------------------------------------------------
void SPI0Setup( void )
{

	// enable spi
	// enable the clock of the SPI0
	AT91F_SPI0_CfgPMC();
	AT91F_SPI_Reset( AT91C_BASE_SPI0 );

	// set SPI master mode before configuring CS (errata 40.4.4 clock can toggle before first transfer)
	AT91F_SPI_CfgMode( AT91C_BASE_SPI0, AT91C_SPI_MSTR );

	// set configuration for CS0
	AT91F_SPI_CfgCs( AT91C_BASE_SPI0,
			0, // CS0
			AT91C_SPI_CPOL |
			AT91C_SPI_BITS_8 |
			(AT91C_SPI_DLYBS & (10 << 16)) | // 10 / mck, 10 / 47923200 = 209nS
			(AT91C_SPI_SCBR & (8 << 8)) ); // mck / 8, 47.923200 / 8 = 5.9904MHz

	// set configuration for CS1
	AT91F_SPI_CfgCs( AT91C_BASE_SPI0,
			1, // CS1
			AT91C_SPI_CPOL |
			AT91C_SPI_BITS_8 |
			(AT91C_SPI_DLYBS & (10 << 16)) | // 10 / mck, 10 / 47923200 = 209nS
			(AT91C_SPI_SCBR & (8 << 8)) ); // mck / 8, 47.923200 / 8 = 5.9904MHz

	AT91F_SPI_Enable( AT91C_BASE_SPI0 );

	// clear PDC buffers of SPI0
	AT91F_PDC_SetNextTx( AT91C_BASE_PDC_SPI0, (char *) 0, 0);
	AT91F_PDC_SetNextRx( AT91C_BASE_PDC_SPI0, (char *) 0, 0);
	AT91F_PDC_SetTx( AT91C_BASE_PDC_SPI0, (char *) 0, 0);
	AT91F_PDC_SetRx( AT91C_BASE_PDC_SPI0, (char *) 0, 0);

	AT91F_PDC_EnableTx( AT91C_BASE_PDC_SPI0 ); // enable DMA transfers

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : InitilizeDisplay
// -----------------------------------------------------------
void InitilizeDisplay( void )
{
	unsigned int i;
	unsigned char command[24];

	// reset display
	command[0] = 0xe2;
	SendToDisplay( command, 1, DM_COMMAND );

	// wait at least 20ms after power on before initializing display - Reset Could be running
	vTaskDelay( 20 / portTICK_RATE_MS ); // wait 20ms

	// initialize display
	command[0] = 0x2c | DISPLAY_DEFAULT_CHARGE_PUMP; // set Internal Charge Pump
	command[1] = 0x28 | DISPLAY_DEFAULT_PANEL_LOADING; // set Panel Loading
	command[2] = 0xe8 | DISPLAY_DEFAULT_BIAS_RATIO; // set Bias Ratio
	command[3] = 0x24 | DISPLAY_DEFAULT_TEMPCOMP; // set Temperature Compensation
	command[4] = 0x81; // set Gain/Pot (2 bytes 0x81 + 'GGPPPPPP')
	command[5] = DISPLAY_DEFAULT_GAIN | pRun.pr.Contrast[pRun.pr.DayNight];
	command[6] = 0x84 | DISPLAY_DEFAULT_PARTIAL; // sets Mux Rate depending on DST/DEN
	command[7] = 0xd0 | DISPLAY_DEFAULT_GRAY_SCALE; // set gray scale mode
	command[8] = 0xa0 | DISPLAY_DEFAULT_LINE_RATE;  // set line rate
	command[9] = 0xf2;  // set DST (2 bytes 0xf2 + 0-159)
	command[10] = DISPLAY_DEFAULT_STARTLINE;
	command[11] = 0xf3; // set DEN (2 bytes 0xf3 + 0-159)
	command[12] = DISPLAY_DEFAULT_ENDLINE;
	command[13] = 0xf1;  // set CEN (2 bytes 0xf1 + 0-159)
	command[14] = DISPLAY_DEFAULT_COMEND;
	command[15] = 0xc0 | DISPLAY_DEFAULT_MAPPING;  // set LCD mapping MY/MX/MSF
	command[16] = 0x88 | DISPLAY_DEFAULT_RAMCONTROL;  // set RAM address control
	command[17] = 0x32; // set MAX column address (2 bytes 0x32 + 0-239)
	command[18] = DISPLAY_DEFAULT_MAXCOLUMN;
	command[19] = 0xa8 | DISPLAY_DEFAULT_COLUMNS; // turn on seg drivers (n x 80columns)
	// send display setup commands - but don't turn display on yet
	SendToDisplay( command, 20, DM_COMMAND );

	// clear global display buffer
	for( i = 0; i < GRAPHICS_SIZE; i++ )
	{
		adm[i] = 0x00;
	}

	// wait until SPI PDC ready
	while( !(AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXBUFE) )
	{
	}
	// clear display driver memory - before turning display on
	WriteDisplay( adm );

	// turn display on after clearing display memory
	command[0] = 0xa8 | DISPLAY_DEFAULT_COLUMNS; // turn on seg drivers (n x 80columns)
	SendToDisplay( command, 1, DM_COMMAND );

	// wait until SPI PDC ready
	while( !(AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXBUFE) )
	{
	}

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : vDisplayTask
// -----------------------------------------------------------
static void vDisplayTask( void *pvParameters )
{
    unsigned int lastkey;
    unsigned char count;
    
    portTickType xLastWakeTime;

    void (*old_window)( unsigned int key );

    // wait at least 15ms after power on before initializing display - internal Reset could be running
    vTaskDelay( 15 / portTICK_RATE_MS ); // wait 15ms

    // setup SPI0 for display
    
    SPI0Setup();

    // setup display
    InitilizeDisplay();

    /* if Brightness is Zero PIO mode is used, else peripheral PWM is used, Backlight run at 500Hz */
    SetOutputPWM( BACK, 500, pRun.pr.Brightness[pRun.pr.DayNight] );
    ResetSequenceFactors( SEQ_ALL );
    // watchdog time is n * (128 / 32768) if slow clock SCK = 32768 - max is a 12 bit value of 0xfff
    // watchdog mode = reset on, debug halt on, delta 1.0 sec, watchdog time 1.0 sec
    AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDRSTEN | AT91C_WDTC_WDDBGHLT | (256 << 16) | (256 << 0);
    // AT91C_BASE_WDTC->WDTC_WDMR= AT91C_WDTC_WDDIS; // Watchdog Disable

    // set current time
    xLastWakeTime = xTaskGetTickCount();

    while( 1 )
    {
        vTaskDelayUntil( &xLastWakeTime, (100 / portTICK_RATE_MS) ); // task executed every 100 ms

        key = (~AT91C_BASE_PIOB->PIO_PDSR) & SW_MASK; // AT91F_PIO_GetInput(AT91C_BASE_PIOB)

        // --- Hardware specific functions above ---
#else
        // --- Windows specific functions --- 
        void sim_startup( void )
        {

        	window = screen00; // Startup Splash Screen
        	looptime = 0;
        }

        extern void WriteToScreen( void );

        void sim_main( unsigned int keypress )
        {
        	static unsigned int lastkey;
        	static unsigned char count;

        	//unsigned char tstring[16];

        	void (*old_window)( unsigned int keypress );

        	if( keypress != (int)-1 )
        	{
        		key = keypress;
        	}
        	else
        	{
        		key = lastkey & ~(SW_NEW | SW_REP);
        	}

#endif

// Every 100 ms

#ifndef WINDOWS  
        	// Calculate Analogue Input Rolling Averages:
        	// Bale Diameter (AnIP1); (Bale) Density Pressure (AnIP2); Bale Shape Indicator (AnIP3); Knife Pressure (AnIP4)
        	CalculateAnalogInputRollingAverages();

        	SupplyVolts =  ((float)AT91C_BASE_ADC0->ADC_CDR7 * ATODIPVOUTSCALE) + ATODIPVOUTDROP;
#endif

        	if( key != (lastkey & ~(SW_NEW | SW_REP)) ) /* different key: could be a keypressed or no key */
        	{
        		key |= SW_NEW;
        		if( key & ~(SW_NEW | SW_REP))
        		{
        			buzz_length = KEY_BEEP; // 48 mS beep if new key pressed
        			looptime = 0;
        		}
        		else /* no key */
        		{
        			swrel = 0;
        		}
        		count = 5;     // 5 counts for first key
        	}
        	else if( count )
        	{
        		count--;
        	}
        	else if( key )
        	{
        		key |= SW_REP;
        		count = 1;
        	}
        	lastkey = key;

        	old_window = window;

        	half_second = (looptime / 5) & 0x01;

        	window( key ); // screens called here

        	looptime++; //incremented every 100 ms

        	if( looptime > 239 )
        	{
        		looptime = 0;
        	}

        	swrel++; //incremented every 100 ms
        	if( swrel > 239 )
        	{
        		swrel = 0;
        	}

        	if( !((window == mainscreen) && SecondScreen) &&  // Dont turn off in Main/Home Screen 2
        			(window != screen100) &&                      // Dont turn off in Machine Menu
        			(window != screen110) &&                      // Dont turn off in Machine Menu - Bale Setup Menu
        			(window != screen111) &&                      // Dont turn off in Machine Menu - Machine Setup Menu
        			(window != screen112) &&                      // Dont turn off in Machine Menu - Bale Count Menu
        			(window != screen113) &&                      // Dont turn off in Machine Menu - Operator Setup Menu
        			(window != screen120) &&                      // Dont turn off in Lube Count Reset Screen
        			(window != screen121) &&                      // Dont turn off in Bale Count Reset Screen
#if 0 // Removed -- Dia pot zero value fixed
        			(window != screen123) &&                      // Dont turn off in Dia Pot Zero Reset Screen
#endif
        			(window != screen124) &&                      // Dont turn off in Fill Pot Zero Reset Screen (AKA Bale Shape Indicator Zero)
        			(window != screen125) &&                      // Dont turn off in Net Metres Reset Screen
        			(window != screen131) &&                      // Dont turn off in Operator Setup Menu - Brightness & Contrast Adjust Menu
        			(window != screen132) &&                      // Dont turn off in Operator Setup Menu - Time & Date Adjust Menu
        			(window != screen140) &&                      // Dont turn off in Technician Menu - PIN entry
        			(window != screen141) &&                      // Dont turn off in Technician Menu
        			(window != screen142) &&                      // Dont turn off in Technician Menu - Baler Full
        			(window != screen143) &&                      // Dont turn off in Technician Menu - Netting Setup
        			(window != screen144) &&                      // Dont turn off in Technician Menu - Diameter Setup
        			(window != screen145) &&                      // Dont turn off in Technician Menu - Density Setup
        			(window != screen146) &&                      // Dont turn off in Technician Menu - Knives Setup
        			(window != screen147) &&                      // Dont turn off in Technician Menu - Reset Defaults
        			(window != screen148) &&                      // Dont turn off in Technician Menu - Formation 2
        			(window != screen151) )                       // Dont turn off in Diagnostics - Outputs
        	{
        		if( key & SW_OL )
        		{
        			offtime++;

        			if( offtime == 8 )
        			{
        				window = screen99;
        			}
        		}
        		else
        		{
        			offtime = 0;
        		}
        	}

        	if( !key )
        	{
        		if( reverttime < 250 )
        		{
        			reverttime++;
        		}

        		if( reverttime >= 200 )
        		{
        			if( (window != mainscreen) &&  // Not Main/Home Screen
#if 0 // Restoring automatic return to Main screen after period of inactivity
        					(window != screen100) &&   // Not in Machine Menu
        					(window != screen110) &&   // Not in Bale Setup Menu
        					(window != screen111) &&   // Not in Machine Setup Menu
        					(window != screen112) &&   // Not in Bale Count Menu
        					(window != screen113) &&   // Not in Operator Setup Menu
#endif
        					(window != screen120)  &&  // Not in Lube Count Reset Screen
        					(window != screen121)  &&  // Not in Bale Count Reset Screen
        					(window != screen124)  &&  // Not in Fill Pot Zero Reset Screen (AKA Bale Shape Indicator Zero)
        					(window != screen125)  &&  // Not in Net Metres Reset Screen
#if 0 // Restoring automatic return to Main screen after period of inactivity
        					(window != screen131)  &&  // Not in Brightness & Contrast Adjust Menu
        					(window != screen132)  &&  // Not in Time & Date Adjust Menu
        					(window != screen140)  &&  // Not Technician Menu - PIN entry
#endif
        					(window != screen141)  &&  // Not Technician Menu
        					(window != screen142)  &&  // Not Technician Menu - Baler Full
        					(window != screen143)  &&  // Not Technician Menu - Netting Setup
        					(window != screen144)  &&  // Not Technician Menu - Diameter Setup
        					(window != screen145)  &&  // Not Technician Menu - Density Setup
        					(window != screen146)  &&  // Not Technician Menu - Knives Setup
        					(window != screen147)  &&  // Not Technician Menu - Reset Defaults
        					(window != screen148)  &&  // Not Technician Menu - Formation 2
        					(window != screen150)  &&  // Not Diagnostics - Digital Inputs
        					(window != screen152)  &&  // Not Diagnostics - Analogue Inputs
        					(window != screen151)    ) // Not Diagnostics - Outputs
        			{
        				window = mainscreen; // Main/Home Screen
        				nextwindow = 0;
        				editing = 0;
        				leaf = 0;
        				cont_beep = 1;
        				buzz_length = BACK_TO_MAIN_SCR_BEEP;
        			}
        			else if( (window == mainscreen) && SecondScreen ) // Main/Home Screen 2
        			{
        				SecondScreen = 0; // Return to Screen 1
        				cont_beep = 1;
        				buzz_length = BACK_TO_MAIN_SCR_BEEP;
        			}
        		}
        		else if( reverttime >= 20 )
        		{
        			if( window == screen99 )
        			{
        				window = mainscreen; // Main/Home Screen
        				nextwindow = 0;
        				editing = 0;
        				leaf = 0;
        			}
        		}
        	}
        	else if( key )
        	{
        		reverttime = 0;
        	}

        	if( deftime )
        	{
        		deftime--;
        		if( deftime == 0 )
        		{
        			window = mainscreen; // Main/Home Screen
        		}
        	}

        	if( (window != screen00) && (window != screen150) && (window != screen151) && (window != screen152))
        	{
        		Inputs();

#ifndef WINDOWS
        		CheckAlarms();
#endif

        		Sequence();

        	}

        	if( escape )
        	{
        		escape = 0;
        		if( windowdepth )
        		{
        			windowdepth--;
        			window = PreviousWindow[windowdepth].window;
        			selection = PreviousWindow[windowdepth].selection;
        		}
        		else
        		{
        			window = mainscreen; // Main/Home Screen
        			selection = 0;
        		}
        	}
        	// window has changed
        	else if( window != old_window )
        	{
        		// save previous window if leaf clear & nextwindow is 0 or NEXTWINDOW_ESCAPE
        		if( !leaf && (nextwindow != NEXTWINDOW_NOESCAPE) )
        		{
        			PreviousWindow[windowdepth].window = old_window;
        			PreviousWindow[windowdepth].selection = selection;
        			if( windowdepth < (MAX_WINDOWDEPTH - 1) )
        			{
        				windowdepth++;
        			}
        		}
        		selection = 0;
        	}

        	if( window != old_window )
        	{
        		startup = 0;
        		nextwindow = 0;
        		editing = 0;
        		leaf = 0;

        		key &= ~(SW_NEW | SW_REP);
        		window( 0 ); // no key action
        	}

#ifndef WINDOWS  

        	WriteDisplay( adm );

        	// restart watchdog counter
        	AT91C_BASE_WDTC->WDTC_WDCR = WATCHDOG_KEY | AT91C_WDTC_WDRSTT;

        } // end of while( 1 )
#else

        WriteToScreen();

#endif 

    }
// -----------------------------------------------------------


#ifndef WINDOWS

// -----------------------------------------------------------
// Function Name       : vStartDisplayTask
// -----------------------------------------------------------
void vStartDisplayTask( unsigned portBASE_TYPE uxPriority )
{

	/* Spawn the task. */
	xTaskCreate( vDisplayTask, "DISPLAY", configDISPLAY_STACK_SIZE, (void *)NULL, uxPriority, (xTaskHandle *)NULL );
}
// -----------------------------------------------------------

#endif

