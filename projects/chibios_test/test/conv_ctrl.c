

#include "conv_ctrl.h"
#include "hal.h"
#include "chprintf.h"

#define CONV_PORT          GPIOA
#define CONV_BOOST_PIN     0
#define CONV_BUCK_PIN      1

#define CONV_PWM           PWMD2
#define PWM_BUCK_CHAN      1
#define PWM_BOOST_CHAN     0

#define CONV_ADC_PORT      GPIOA
#define CONV_INPUT_FB_PIN  2
#define CONV_BOOST_FB_PIN  3
#define CONV_BUCK_FB_PIN   4

#define ADC_NUM_CHANNELS   3
#define ADC_BUF_DEPTH      2
#define INPUT_IND          0
#define BOOST_IND          1
#define BUCK_IND           2

static uint16_t buckPwm   = 0;
static uint16_t boostPwm  = 0;
static uint16_t buckGain  = 1000;
static uint16_t boostGain = 1000;
static uint16_t buckSp    = 2068;
static uint16_t boostSp   = 3061;
static uint16_t inputSp   = 2275;

static void contAdcReadyCb( ADCDriver * adcp, adcsample_t * buffer, size_t n )
{
    (void)adcp;
    //(void)buffer;
    (void)n;
    // Buck
    if ( buffer[ BUCK_IND ] < buckSp )
    {
    	buckPwm += buckGain;
    	buckPwm = ( buckPwm <= 10000 ) ? buckPwm : 10000;
        pwmEnableChannelI(&CONV_PWM, PWM_BUCK_CHAN, PWM_PERCENTAGE_TO_WIDTH( &CONV_PWM, buckPwm ) );
    }
    else if ( buffer[ BUCK_IND ] > buckSp )
    {
    	buckPwm = ( buckPwm >= buckGain ) ? buckPwm : buckGain;
    	buckPwm -= buckGain;
    	pwmEnableChannelI(&CONV_PWM, PWM_BUCK_CHAN, PWM_PERCENTAGE_TO_WIDTH( &CONV_PWM, buckPwm ) );
    }
    // Boost
    if ( buffer[ INPUT_IND ] >= inputSp )
    {
        if ( buffer[ BOOST_IND ] < boostSp )
        {
        	boostPwm += boostGain;
        	boostPwm = ( boostPwm <= 9000 ) ? boostPwm : 9000;
        	pwmEnableChannelI(&CONV_PWM, PWM_BOOST_CHAN, PWM_PERCENTAGE_TO_WIDTH( &CONV_PWM, boostPwm ) );
        }
        else if ( buffer[ BOOST_IND ] > boostSp )
        {
        	boostPwm = ( boostPwm >= boostGain ) ? boostPwm : boostGain;
        	boostPwm -= boostGain;
        	pwmEnableChannelI(&CONV_PWM, PWM_BOOST_CHAN, PWM_PERCENTAGE_TO_WIDTH( &CONV_PWM, boostPwm ) );
        }
    }
    else
    	pwmEnableChannelI(&CONV_PWM, PWM_BOOST_CHAN, PWM_PERCENTAGE_TO_WIDTH( &CONV_PWM, 0 ) );
};

static adcsample_t adcSamples[ ADC_NUM_CHANNELS * ADC_BUF_DEPTH ];

// ADC_SAMPLE_1P5
// ADC_SAMPLE_7P5
// ADC_SAMPLE_13P5
// ADC_SAMPLE_28P5
// ADC_SAMPLE_41P5
// ADC_SAMPLE_55P5
// ADC_SAMPLE_71P5
// ADC_SAMPLE_239P5

static const ADCConversionGroup adcGroup =
{
    TRUE,
    ADC_NUM_CHANNELS,
    contAdcReadyCb,
    NULL,
    0, 0,           /* CR1, CR2 */
    0,
    ADC_SMPR2_SMP_AN2(ADC_SAMPLE_7P5) | ADC_SMPR2_SMP_AN3( ADC_SAMPLE_7P5 ) |
    ADC_SMPR2_SMP_AN4( ADC_SAMPLE_7P5 ),
    ADC_SQR1_NUM_CH( ADC_NUM_CHANNELS ),
    0,
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN2) |
    ADC_SQR3_SQ2_N(ADC_CHANNEL_IN3) | 
    ADC_SQR3_SQ3_N(ADC_CHANNEL_IN4)
};

static PWMConfig pwmCfg =
{
    48000000, // 1000kHz PWM clock frequency.
    480,      // Initial PWM period 10us.
    NULL,
    {
        { PWM_OUTPUT_ACTIVE_HIGH, NULL },
        { PWM_OUTPUT_ACTIVE_HIGH, NULL },
        { PWM_OUTPUT_DISABLED, NULL },
        { PWM_OUTPUT_DISABLED, NULL }
    },
    0,
#if STM32_PWM_USE_ADVANCED
    0
#endif
};




void convStart( void )
{
    /*
    pwmStart( &PWMD2, &pwmCfg );
    palSetPadMode( GPIOA, 0, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    palSetPadMode( GPIOA, 1, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    //palSetPadMode( GPIOA, 2, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH( &PWMD2, 9500 ) );
    pwmEnableChannel(&PWMD2, 1, PWM_PERCENTAGE_TO_WIDTH( &PWMD2, 3030 ) );
    //pwmEnableChannel(&PWMD2, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD2, 3030 ) );
    */

	// Start PWM peripherial.
    pwmStart( &CONV_PWM, &pwmCfg );
	// Init PWM pins.
    palSetPadMode( CONV_PORT, CONV_BUCK_PIN,  PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    palSetPadMode( CONV_PORT, CONV_BOOST_PIN, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    // Set zero active period.
    pwmEnableChannel(&CONV_PWM, PWM_BOOST_CHAN, PWM_PERCENTAGE_TO_WIDTH( &CONV_PWM, 0000 ) );
    pwmEnableChannel(&CONV_PWM, PWM_BUCK_CHAN,  PWM_PERCENTAGE_TO_WIDTH( &CONV_PWM, 0000 ) );

    // Init ADC.
    palSetGroupMode(CONV_ADC_PORT, PAL_PORT_BIT( CONV_BUCK_FB_PIN ) |
    	                           PAL_PORT_BIT( CONV_BOOST_FB_PIN ) |
    	                           PAL_PORT_BIT( CONV_INPUT_FB_PIN ),
                                   0, PAL_MODE_INPUT_ANALOG);
    adcStart( &ADCD1, NULL );

    adcStartConversion( &ADCD1, &adcGroup, adcSamples, ADC_BUF_DEPTH );
}

void convStop( void )
{
	adcStopConversion( &ADCD1 );
	adcStop( &ADCD1 );
    pwmStop( &CONV_PWM );
}

void convSetBuck( uint16_t sp )
{
    buckSp = sp;
}

void convSetBoost( uint16_t sp )
{
    boostSp = sp;
}

void convSetInput( uint16_t minValue )
{
    inputSp = minValue;
}

void convSetBuckGain( uint16_t val )
{
    buckGain = val;
}

void convSetBoostGain( uint16_t val )
{
    boostGain = val;
}

void cmd_conv( BaseChannel *chp, int argc, char * argv [] )
{
	(void)argc;
	(void)argv;
	chprintf( chp, "{%d, %d, %d}\r\n", adcSamples[0], adcSamples[1], adcSamples[2] );
}





