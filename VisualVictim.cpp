#include "VisualVictim.h"
void CameraData::read(int height, int width, void * mom){
  ID = "";
  victim = '\0';
  img = Mat(height, width, CV_8UC4, mom);
  
  Mat frame = img.clone();
  //grayscale
  cvtColor(frame,frame,COLOR_BGR2GRAY);
  //blur
  GaussianBlur(frame, frame, Size(1, 1), 0, 0);
  //threshold
  threshold(frame, frame, 127, 255, THRESH_BINARY);
  //find contours
  std::vector<std::vector<Point>> contours;
  findContours(frame, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
  //cycle through contours to find valid victims
  for(int i = 0; i < (int)contours.size(); i++){
    Rect r = boundingRect(contours[i]);
    std::cout<<r.height<<' '<<r.width<<std::endl;
    if(r.y < r.height/2 && r.height * r.width >= 1200 && r.height * r.width < 6400 && abs((double)r.height / (double)r.width - 1) <= 0.1){    
        if(r.x >= 0 && r.x + r.width <= img.cols && r.y >= 0 && r.y + r.height <= img.rows){
        rectangle(img, r, Scalar(0, 255, 0));
        Mat slice = frame(r);
        threshold(slice, slice, 110, 255, THRESH_BINARY_INV);
        for(int i = 0; i < 3; i++){
          Mat slicer = slice(Rect(FRAME_CROP, (slice.rows - FRAME_CROP) * i / 3, slice.cols - FRAME_CROP, (slice.rows - FRAME_CROP) / 3));
          findContours(slicer, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);       
          ID += std::to_string(contours.size());
        }
        if(ID == "212")
          victim = 'H';
        else if(ID == "121")
          victim = 'S';
        else if(ID == "221")
          victim = 'U';
        else 
          victim = '\0';
        break;
        }
      }
  }
}

void CameraData::display(std::string s){
  imshow(s, img);
  std::cout<<s<<": "<<victim<<std::endl;
  waitKey(1);
}