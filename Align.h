// -----------------------------------------------------------------------------------
// GEOMETRIC ALIGN FOR ALT/AZM AND EQ MOUNTS
//
// by Howard Dutton
//
// Copyright (C) 2012 to 2018 Howard Dutton
//

#pragma once

byte alignNumStars = 0;
byte alignThisStar = 0;

// -----------------------------------------------------------------------------------
// ADVANCED GEOMETRIC ALIGN FOR ALT/AZM MOUNTS (GOTO ASSIST)

#ifdef MOUNT_TYPE_ALTAZM

typedef struct {
  double ha;
  double dec;
  double alt;
  double azm;
  int side;
} align_coord2_t;

class TGeoAlignH
{
  public:
    double ax1Cor;
    double ax2Cor;
    double altCor;
    double azmCor;
    double doCor;
    double pdCor;
    double dfCor;
    double tfCor;
    align_coord2_t mount[9];
    align_coord2_t actual[9];
    align_coord2_t delta[9];

    void init();
    void readCoe();
    void writeCoe();
    bool isReady();
    bool addStar(int I, int N, double RA, double Dec);
    void HorToInstr(double Alt, double Azm, double *Alt1, double *Azm1, int PierSide);
    void InstrToHor(double Alt, double Azm, double *Alt1, double *Azm1, int PierSide);
    void autoModel(int n, bool start);

  private:
    boolean geo_ready;
    double avgAlt;
    double avgAzm;

    double lat,cosLat,sinLat;

    long num,l;
    long Ff,Df;
    double best_deo, best_pd, best_pz, best_pe, best_ohw, best_odw, best_ohe, best_ode, best_tf, best_df, best_ff;
    double z1,a1;
    double avg_azm,avg_alt;
    double dist,sumd,rms;
    double best_dist;
    double ohe,ode,ohw,odw,dh;
    double sa,sz,sum1;
    double max_dist;

    void correct(double azm, double alt, double pierSide, double sf, double _deo, double _pd, double _pz, double _pe, double _da, double _ff, double _tf, double *z1, double *a1);
    void do_search(double sf, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9);
};

TGeoAlignH Align;
#endif

// -----------------------------------------------------------------------------------
// ADVANCED GEOMETRIC ALIGN FOR EQUATORIAL MOUNTS (GOTO ASSIST)

#ifndef MOUNT_TYPE_ALTAZM

typedef struct {
  double ha;
  double dec;
  int side;
} align_coord2_t;

class TGeoAlign
{
  public:
    double ax1Cor;
    double ax2Cor;
    double altCor;
    double azmCor;
    double doCor;
    double pdCor;
    double dfCor;
    double tfCor;
    align_coord2_t mount[9];
    align_coord2_t actual[9];
    align_coord2_t delta[9];

    void init();
    void readCoe();
    void writeCoe();
    bool isReady();
    bool addStar(int I, int N, double RA, double Dec);
    void EquToInstr(double Lat, double HA, double Dec, double *HA1, double *Dec1, int PierSide);
    void InstrToEqu(double Lat, double HA, double Dec, double *HA1, double *Dec1, int PierSide);
    void autoModel(int n, bool start);

  private:
    boolean geo_ready;
    double avgDec;
    double avgHA;

    double lat,cosLat,sinLat;

    long num,l;
    long Ff,Df;
    double best_deo, best_pd, best_pz, best_pe, best_ohw, best_odw, best_ohe, best_ode, best_tf, best_df, best_ff;
    double h1,d1;
    double avg_ha,avg_dec;
    double dist,sumd,rms;
    double best_dist;
    double ohe,ode,ohw,odw,dh;
    double sd,sh,sum1;
    double max_dist;

    void correct(double ha, double dec, double pierSide, double sf, double _deo, double _pd, double _pz, double _pe, double _da, double _ff, double _tf, double *h1, double *d1);
    void do_search(double sf, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9);
};

TGeoAlign Align;
#endif

