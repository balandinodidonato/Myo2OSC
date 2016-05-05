int labelTextSize = int(width*0.0303030303);
int labeX = int(width*0.006666666667);
int labelXFactor = int(height*0.05681818182);
int marginY = int(height*0.05681818182);
int[] labelY = new int[18];
int graphLabelsTextSize;

void labels(){
  graphLabelsTextSize = int(height*0.033);
  int textXheader = int(width*0.615);
  int textXfooter1 = int(textXheader+(textXheader*0.05));
  int textXfooter2 = int(textXheader+(textXheader*0.044));

  textSize(graphLabelsTextSize*2);
  fill(#1D4383);
  text("MyoMapper", textXheader,height*0.06);
  
  textSize(graphLabelsTextSize);
  text("POSE: "+pose, rEmgX,rEmgY+rEmgYsize*2); 
  text("STATE: "+connectionState+" - "+syncState, rEmgX,rEmgY+rEmgYsize*3);
  
  textSize(graphLabelsTextSize*0.6);
  text("DATA MAPPING", tglemgX-width*0.017, tglemgY-tglYsize*0.5);
  text("RESCALE", sYawX+(tglYawX*0.25), tglemgY-tglYsize*0.5);
  text("Balandino Di Donato - 2016", textXfooter1,height*0.97); 
  text("www.balandinodidonato.com",  textXfooter2, height*0.99);
  
  noFill(); 

}

void labelsAnalysis(){
  
  int X0 = 0;
  int X1 = int(width*0.52);
  int Y1 = int(height*0.516);
  int factor = int(height*0.168);
  
 
  stroke(96, 170, 210);

  for(int i=0; i<3; i++){
    int Y12 = int(Y1+(i*factor));
    line(X0, Y12, X1, Y12);
  }
  
  noStroke();

  for(int i=0; i<18; i++){
    labelY[i]= int(height*0.0555555555)*(i+1)-int(height*0.0055);
    
    stroke(96, 170, 210);
    if(i<8) line(X0, labelY[i]+int(height*0.01), X1, labelY[i]+int(height*0.01));   
    noStroke();

    textSize(graphLabelsTextSize);
    fill(155);
    for(int n = 0; n<8; n++)text("EMG "+(n+1), labeX, labelY[n]);

    fill(255);
    text("  MAV", labeX, labelY[8]);
  
    fill(255, 0, 0);
    text(" YAW", labeX, labelY[9]);
    fill(0, 255, 0);
    text("PITCH", labeX, labelY[10]);
    fill(0, 0, 255);
    text(" ROLL", labeX, labelY[11]);
    fill(255, 0, 0);
  
    text("ACC X", labeX, labelY[12]);
    fill(0, 255, 0);
    text("ACC Y", labeX, labelY[13]);
    fill(0, 0, 255);
    text("ACC Z", labeX, labelY[14]);
    fill(255, 0, 0);
  
    text("GYR X", labeX, labelY[15]);
    fill(0, 255, 0);
    text("GYR Y", labeX, labelY[16]);
    fill(0, 0, 255);
    text("GYR Z", labeX, labelY[17]);
    }
    
  noFill();
  noStroke();
}