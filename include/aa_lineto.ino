void lineto(double x, double y, double MovingSpeed)
{
double DX=x-blockx;
double DY=y-blocky;
double L=sqrt(DX*DX+DY*DY);
double steptime = steplength/MovingSpeed;
double dx=DX*steplength/L;
double dy=DY*steplength/L;
int nsteps=(int)(L/steplength);               // geheel aantal stappen dat nodig is
double rest=L-nsteps*steplength;              // het restgedeelte van de beweging (< 1 stap)

// handel eerst alle volledige stappen af:
//for(int s=0;s<nsteps;s++){
 // blockx+=dx;
 // blocky+=dy;
  //grid->drawDot(blockx,blocky,5,clRed);     // UPDATE HIER DE 8 KABELLENGTES
//  Sleep((int)(steptime*1000.0));
 // }
// handel tenslotte de rest af:
//if(rest>0.0){
 // blockx=x;
 // blocky=y;
  //grid->drawDot(blockx,blocky,5,clRed);     // UPDATE HIER DE 8 KABELLENGTES
 // Sleep((int)(steptime*1000.0));
 // }
}
