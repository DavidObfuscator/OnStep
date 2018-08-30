// -------------------------------------------------------------------------------------------------------
// Handle encoders, both CW/CCW and Quadrature A/B types are supported

#if (!defined(AXIS1_ENC_OFF) && !defined(AXIS2_ENC_OFF))
  #define ENCODERS_ON
#endif

// this is for Quadrature type encoders -------------------
#if defined(AXIS1_ENC_AB) || defined(AXIS2_ENC_AB)
  #include <Encoder.h> // from https://github.com/PaulStoffregen/Encoder 
#endif
#ifdef AXIS1_ENC_AB
  Encoder axis1Pos(AXIS1_ENC_A_PIN,AXIS1_ENC_B_PIN);
#endif
#ifdef AXIS2_ENC_AB
  Encoder axis2Pos(AXIS2_ENC_A_PIN,AXIS2_ENC_B_PIN);
#endif
// --------------------------------------------------------

// this is for CW/CCW type encoders -----------------------
#if defined(AXIS1_ENC_CWCCW) || defined(AXIS2_ENC_CWCCW)
  volatile int32_t __p1;
  void __cw1() { __p1++; }
  void __ccw1() { __p1--; }

  volatile int32_t __p2;
  void __cw2() { __p2++; }
  void __ccw2() { __p2--; }
  
  class CwCcwEncoder {
    public:
      CwCcwEncoder(int16_t cwPin, int16_t ccwPin, int16_t axis) {
        _cwPin=cwPin;
        _ccwPin=ccwPin;
        _axis=axis;
        pinMode(_cwPin,INPUT_PULLUP);
        pinMode(_ccwPin,INPUT_PULLUP);
        if (_axis==1) {
          attachInterrupt(digitalPinToInterrupt(_cwPin),__cw1,CHANGE);
          attachInterrupt(digitalPinToInterrupt(_ccwPin),__ccw1,CHANGE);
        }
        if (_axis==2) {
          attachInterrupt(digitalPinToInterrupt(_cwPin),__cw2,CHANGE);
          attachInterrupt(digitalPinToInterrupt(_ccwPin),__ccw2,CHANGE);
        }
      }
      int32_t read() {
        int32_t v=0;
        if (_axis==1) { noInterrupts(); v=__p1; interrupts(); }
        if (_axis==2) { noInterrupts(); v=__p2; interrupts(); }
        return v;
      }
      void write(int32_t v) {
        if (_axis==1) { noInterrupts(); __p1=v; interrupts(); }
        if (_axis==2) { noInterrupts(); __p2=v; interrupts(); }
      }
    private:
      int16_t _cwPin;
      int16_t _ccwPin;
      int16_t _axis;
  };
#endif
#ifdef AXIS1_ENC_CWCCW
  CwCcwEncoder axis1Pos(AXIS1_ENC_A_PIN,AXIS1_ENC_B_PIN,1);
#endif
#ifdef AXIS2_ENC_CWCCW
  CwCcwEncoder axis2Pos(AXIS2_ENC_A_PIN,AXIS2_ENC_B_PIN,2);
#endif
// --------------------------------------------------------

class Encoders {
  public:
    void init() {
    }
    void poll() {
#ifdef ENCODERS_ON
      // check encoders and sync OnStep if diff is too great, checks every 2 seconds
      static unsigned long nextEncCheckMs=millis()+2000UL;
      unsigned long temp=millis();
      char *conv_end;
      if ((long)(temp-nextEncCheckMs)>0) {
        nextEncCheckMs=temp+2000UL;
        char s[22];
        Ser.print(":GX42#"); s[Ser.readBytesUntil('#',s,20)]=0;
        if (strlen(s)>1) { // can return error code "0" also, which is ignored
          double f=strtod(s,&conv_end);
          if ( (&s[0]!=conv_end) && (f>=-999.9) && (f<=999.9)) _osAxis1=f;
        }
        Ser.print(":GX43#"); s[Ser.readBytesUntil('#',s,20)]=0;
        if (strlen(s)>1) { // can return error code "0" also, which is ignored
          double f=strtod(s,&conv_end);
          if ( (&s[0]!=conv_end) && (f>=-999.9) && (f<=999.9)) _osAxis2=f;
        }
        _enAxis1=(double)axis1Pos.read()/(double)AXIS1_ENC_TICKS_DEG;
        #ifdef AXIS1_ENC_REVERSE_ON
        _enAxis1=-_enAxis1;
        #endif
        _enAxis2=(double)axis2Pos.read()/(double)AXIS2_ENC_TICKS_DEG;
        #ifdef AXIS1_ENC_REVERSE_ON
        _enAxis2=-_enAxis2;
        #endif
#ifdef ENCODERS_SYNC_ON
          if ((fabs(_osAxis1-_enAxis1)>(double)AXIS1_ENC_DIFF_LIMIT) ||
              (fabs(_osAxis2-_enAxis2)>(double)AXIS2_ENC_DIFF_LIMIT)) {
          Ser.print(":SX40,"); Ser.print(_enAxis1,6); Ser.print("#"); Ser.readBytes(s,1);
          Ser.print(":SX41,"); Ser.print(_enAxis2,6); Ser.print("#"); Ser.readBytes(s,1);
          Ser.print(":SX42,1#"); Ser.readBytes(s,1);
#endif
        }
      }
#endif
    }
  
    double getAxis1() { return _enAxis1; }
    double getAxis2() { return _enAxis2; }
    double getOnStepAxis1() { return _osAxis1; }
    double getOnStepAxis2() { return _osAxis2; }

  private:
    double _osAxis1=0;
    double _osAxis2=0;
    double _enAxis1=0;
    double _enAxis2=0;
};

