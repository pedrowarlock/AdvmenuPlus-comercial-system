/*      __                              __                     
 *     /\ \                            /\ \                    
 *     \_\ \     __       __    ____   \_\ \     __       __   
 *     /'_` \  /'__`\   /'__`\ /',__\  /'_` \  /'__`\   /'__`\ 
 *    /\ \L\ \/\ \L\.\_/\  __//\__, `\/\ \L\ \/\ \L\.\_/\  __/ 
 *    \ \___,_\ \__/.\_\ \____\/\____/\ \___,_\ \__/.\_\ \____\
 *     \/__,_ /\/__/\/_/\/____/\/___/  \/__,_ /\/__/\/_/\/____/
 *
 *
 *    Driver for IPP Arcade joystick (Interfaz Puerto Paralelo).
 *
 *    By daesdae.
 * 
 *    Supports 1 joystick with up to 56 buttons.
 * 
 *    See <http://www.retrovicio.org/tutoriales/interfaz-puerto-paralelo>
 *    for details on how to build the interface.
 *
 *           |  Pin 10   |  Pin 11   |  Pin 12   |  Pin 13   |  Pin 15
 *    -------------------------------------------------------------------
 *    Pin  2 | UP          DOWN        LEFT        RIGHT       button_1
 *    Pin  3 | button_2    button_3    button_4    button_5    button_6
 *    Pin  4 | button_7    button_8    button_9    button_10   button_11
 *    Pin  5 | button_12   button_13   button_14   button_15   button_16
 *    Pin  6 | button_17   button_18   button_19   button_20   button_21
 *    Pin  7 | button_22   button_23   button_24   button_25   button_26
 *    Pin  8 | button_27   button_28   button_29   button_30   button_31
 *    Pin  9 | button_32   button_33   button_34   button_35   button_36
 *    Pin  1 | button_37   button_38   button_39   button_40   button_41
 *    Pin 14 | button_42   button_43   button_44   button_45   button_46
 *    Pin 16 | button_47   button_48   button_49   button_50   button_51
 *    Pin 17 | button_52   button_53   button_54   button_55   button_56
 *  
 */

#include "portable.h"

#include "jipp.h"
#include "log.h"
#include "target.h"

#define DATA_LPT1           0x378
#define STATUS_LPT1         0x379
#define CONTROL_LPT1        0x37A
#define DATA_PORT_INIT      0xFF
#define STATUS_PORT_INVERT  0x80
#define CONTROL_PORT_INVERT 0x0B

#define IPP_JOYSTICKS_MAX 1
#define IPP_JOYSTICK_BUTTONS_MAX 56
#define IPP_JOYSTICK_STICKS_MAX 1
#define IPP_JOYSTICK_AXIS_MAX 2

#define JOYFLAG_DIGITAL 1
#define JOYFLAG_SIGNED 32

struct joystick_button_context {
	const char* name;
	int b;
};

struct joystick_axis_context {
	const char* name;
	int pos;
	int d1, d2;
};

struct joystick_stick_context {
	const char* name;
	int flags;
	int num_axis;
	struct joystick_axis_context axis[IPP_JOYSTICK_AXIS_MAX];
};

struct joystick_item_context {
	int flags;
	int num_sticks;
	struct joystick_stick_context stick[IPP_JOYSTICK_STICKS_MAX];
	int num_buttons;
	struct joystick_button_context button[IPP_JOYSTICK_BUTTONS_MAX];
};

struct joystickb_ipp_context {
	int id;
	int ipp_id;
	target_clock_t last;
	int num_joysticks;
	struct joystick_item_context joy[IPP_JOYSTICKS_MAX];
};

static struct joystickb_ipp_context ipp_state;

static adv_device DEVICE[] = {
	{ "auto", -1, "IPP Arcade - LPT1" },
	{ 0, 0, 0 }
};

void ipp_init(void)
{
	int i;

	ipp_state.num_joysticks = IPP_JOYSTICKS_MAX;

	ipp_state.joy[0].flags = JOYFLAG_DIGITAL | JOYFLAG_SIGNED;
	ipp_state.joy[0].num_sticks = IPP_JOYSTICK_STICKS_MAX;
	ipp_state.joy[0].num_buttons = IPP_JOYSTICK_BUTTONS_MAX;
	ipp_state.joy[0].stick[0].flags = JOYFLAG_DIGITAL | JOYFLAG_SIGNED;
	ipp_state.joy[0].stick[0].num_axis = IPP_JOYSTICK_AXIS_MAX;
	ipp_state.joy[0].stick[0].axis[0].name = "Poticion x";
	ipp_state.joy[0].stick[0].axis[1].name = "Poticion y";
	ipp_state.joy[0].stick[0].name = "Position";

	for (i=0;i<IPP_JOYSTICK_BUTTONS_MAX;i++){
		ipp_state.joy[0].button[i].name = ("Button "+(i+1));
	}

	/* turns the 4 control bits into inputs */

	target_port_set(CONTROL_LPT1, 0x0F ^ CONTROL_PORT_INVERT);

	/* If the parallel port is ECP, ensure it is in standard mode */

	target_port_set(DATA_LPT1 + 0x402, 0x04);
}

void ipp_exit(void)
{
	int i;

	ipp_state.num_joysticks = 0;

	ipp_state.joy[0].flags = 0;
	ipp_state.joy[0].num_sticks = 0;
	ipp_state.joy[0].num_buttons = 0;

	ipp_state.joy[0].stick[0].flags = 0;
	ipp_state.joy[0].stick[0].num_axis = 0;
	ipp_state.joy[0].stick[0].name = "";

	ipp_state.joy[0].stick[0].axis[0].pos = 0;
	ipp_state.joy[0].stick[0].axis[0].d1 = 0;
	ipp_state.joy[0].stick[0].axis[0].d2 = 0;
	ipp_state.joy[0].stick[0].axis[0].name = "";
	ipp_state.joy[0].stick[0].axis[1].pos = 0;
	ipp_state.joy[0].stick[0].axis[1].d1 = 0;
	ipp_state.joy[0].stick[0].axis[1].d2 = 0;
	ipp_state.joy[0].stick[0].axis[1].name = "";
	
	for (i=0;i<IPP_JOYSTICK_BUTTONS_MAX;i++){
		ipp_state.joy[0].button[i].b = 0;
		ipp_state.joy[0].button[i].name = "";
	}

	target_port_set(CONTROL_LPT1, 0 ^ CONTROL_PORT_INVERT);
}

void ipp_poll(void)
{
	unsigned char status;
	int i; //numero de lineas
	
	/**** LINEAS DATA (2, 3, 4, 5, 6, 7, 8, 9) ****/

	for (i=0;i<8;i++) {
		target_port_set(DATA_LPT1 , ~(1 << i));
		status  = target_port_get(STATUS_LPT1) ^ STATUS_PORT_INVERT;

		if (i==0) {
			ipp_state.joy[0].stick[0].axis[0].d1 = (status & 32  ) ? 0 : 1;
			ipp_state.joy[0].stick[0].axis[0].d2 = (status & 16  ) ? 0 : 1;
			ipp_state.joy[0].stick[0].axis[1].d1 = (status & 64  ) ? 0 : 1;
			ipp_state.joy[0].stick[0].axis[1].d2 = (status & 128 ) ? 0 : 1;
			ipp_state.joy[0].stick[0].axis[0].pos = ((status & 128) ? 0 : 128) - ((status & 64) ? 0 : 128);
			ipp_state.joy[0].stick[0].axis[1].pos = ((status & 32 ) ? 0 : 128) - ((status & 16) ? 0 : 128);
		} else {
			ipp_state.joy[0].button[5*i-4].b = (status & 64 ) ? 0 : 1; //pin 10
			ipp_state.joy[0].button[5*i-3].b = (status & 128) ? 0 : 1; //pin 11
			ipp_state.joy[0].button[5*i-2].b = (status & 32 ) ? 0 : 1; //pin 12
			ipp_state.joy[0].button[5*i-1].b = (status & 16 ) ? 0 : 1; //pin 13
		}
		ipp_state.joy[0].button[5*i].b   = (status & 8  ) ? 0 : 1; //pin 15
	}
	
	target_port_set(DATA_LPT1, DATA_PORT_INIT);

	/**** LINEAS CONTROL (1, 14, 16, 17) ****/

	for (i=8;i<12;i++) {
		target_port_set(CONTROL_LPT1, ~(1 << (i - 8)) ^ CONTROL_PORT_INVERT);
		status  = target_port_get(STATUS_LPT1) ^ STATUS_PORT_INVERT; //inportb

		ipp_state.joy[0].button[5*i-4].b = (status & 64 ) ? 0 : 1; //pin 10
		ipp_state.joy[0].button[5*i-3].b = (status & 128) ? 0 : 1; //pin 11
		ipp_state.joy[0].button[5*i-2].b = (status & 32 ) ? 0 : 1; //pin 12
		ipp_state.joy[0].button[5*i-1].b = (status & 16 ) ? 0 : 1; //pin 13
		ipp_state.joy[0].button[5*i].b   = (status & 8  ) ? 0 : 1; //pin 15
	}
	
	target_port_set(CONTROL_LPT1, 0x0F ^ CONTROL_PORT_INVERT);
}

adv_error joystickb_ipp_init(int id)
{
	log_std(("joystickb:ipp: joystickb_ipp_init(id:%d)\n", id));

	ipp_state.id = id;

	ipp_state.ipp_id = id;

	ipp_state.last = target_clock();

	ipp_init();

	return 0;
}

void joystickb_ipp_done(void)
{
	log_std(("joystickb:ipp: joystickb_ipp_done()\n"));

	ipp_exit();
}

unsigned joystickb_ipp_count_get(void)
{
	log_debug(("joystickb:ipp: joystickb_ipp_count_get()\n"));

	return ipp_state.num_joysticks;
}

unsigned joystickb_ipp_stick_count_get(unsigned j)
{
	log_debug(("joystickb:ipp: joystickb_ipp_stick_count_get()\n"));

	return ipp_state.joy[j].num_sticks;
}

unsigned joystickb_ipp_stick_axe_count_get(unsigned j, unsigned s)
{
	log_debug(("joystickb:ipp: joystickb_ipp_stick_axe_count_get()\n"));

	return ipp_state.joy[j].stick[s].num_axis;
}

unsigned joystickb_ipp_button_count_get(unsigned j)
{
	log_debug(("joystickb:ipp: joystickb_ipp_button_count_get()\n"));

	return ipp_state.joy[j].num_buttons;
}

unsigned joystickb_ipp_button_get(unsigned j, unsigned b)
{
	log_debug(("joystickb:ipp: joystickb_ipp_button_get()\n"));

	return ipp_state.joy[j].button[b].b;
}

unsigned joystickb_ipp_stick_axe_digital_get(unsigned j, unsigned s, unsigned a, unsigned d)
{
	log_debug(("joystickb:ipp: joystickb_ipp_stick_axe_digital_get()\n"));

	if (d)
		return ipp_state.joy[j].stick[s].axis[a].d1;
	else
		return ipp_state.joy[j].stick[s].axis[a].d2;

	return 0;
}

int joystickb_ipp_stick_axe_analog_get(unsigned j, unsigned s, unsigned a)
{
	int r;
	log_debug(("joystickb:ipp: joystickb_ipp_stick_axe_analog_get()\n"));

	r = ipp_state.joy[j].stick[s].axis[a].pos;

	r = joystickb_adjust_analog(r, -128, 128);

	return r;
}


void joystickb_ipp_poll(void)
{
	log_debug(("joystickb:ipp: joystickb_ipp_poll()\n"));

	if (ipp_state.id == -1) {
		target_clock_t now = target_clock();
		/* don't poll too frequently */
		if (now - ipp_state.last > TARGET_CLOCKS_PER_SEC / 30) {
			log_debug(("joystickb:ipp: effective poll\n"));
			ipp_state.last = now;
			ipp_poll();
		} else {
			log_debug(("joystickb:ipp: skipped poll\n"));
		}
	} else {
		ipp_poll();
	}
}

unsigned joystickb_ipp_flags(void)
{
	return 0;
}

adv_error joystickb_ipp_load(adv_conf* context)
{
	return 0;
}

void joystickb_ipp_reg(adv_conf* context)
{
}

/***************************************************************************/
/* Driver */

joystickb_driver joystickb_ipp_driver = {
	"ipp",
	DEVICE,
	joystickb_ipp_load,
	joystickb_ipp_reg,
	joystickb_ipp_init,
	joystickb_ipp_done,
	0,
	0,
	joystickb_ipp_flags,
	joystickb_ipp_count_get,
	joystickb_ipp_stick_count_get,
	joystickb_ipp_stick_axe_count_get,
	0,
	0,
	joystickb_ipp_stick_axe_digital_get,
	joystickb_ipp_stick_axe_analog_get,
	joystickb_ipp_button_count_get,
	0,
	joystickb_ipp_button_get,
	0,
	0,
	0,
	0,
	0,
	joystickb_ipp_poll
};

