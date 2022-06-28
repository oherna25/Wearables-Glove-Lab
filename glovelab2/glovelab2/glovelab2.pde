
import g4p_controls.*; // need g4p lib
import processing.serial.*;
import java.awt.Font;
import java.util.Map;
import controlP5.*;
import java.io.Serializable;


HashMap<Integer, PImage> wordMap = new HashMap<Integer, PImage>();
PImage[] imageArray;

int pic_width = 1580;
int pic_height = 2108;
String boardInput;
String[] words;
//int space = 100;
String handInput;


Serial myPort;
public void setup(){
  size(700, 560, JAVA2D);
  createGUI();
  customGUI();
  // Place your setup code here
   myPort = new Serial(this, Serial.list()[0], 115200);
   textAlign(CENTER);
   //background(#28A2D1);
   imageArray = new PImage[25];
   setupWords();
   setupArray();
   setupMap();
   mainMenu();
   
}

void setupWords() {
  
  words = new String[50];
  words[0] = "BIOMEDICAL";
  words[1] = "GREEN";
  words[2] = "RED";
  words[3] = "ORGAN";
  words[5] = "EXTRA";
  words[6] = "IRENE";
  words[7] = "SAM";
  words[8] = "PRIYA";
  words[9] = "SUN RISES";
  words[10] = "HAPPY";
  words[11] = "UIC";
  words[12] = "FUN";
  words[13] = "SENSORS";
  words[14] = "SENSOR";
  words[15] = "ONE TWO THREE";
  words[16] = "HURT";
  words[17] = "SCIENCE";
  words[18] = "JACK";
  words[19] = "BEAT";
  words[20] = "COMPUTER";
  words[21] = "HURT";
  words[22] = "GENES";
  words[23] = "MAD";
  words[23] = "MAGIC";
  
   
}

void setupArray() {
    
    char j = 'A';
    for (int i= 0; i < imageArray.length; i++) {
      imageArray[i] = loadImage( j + ".jpg");
       //println(j);
      j++;
    }
  
}

void setupMap() {
 
     char a = 'A'; 
     for (PImage  i : imageArray) {
       wordMap.put(int(a), i);
       println(int(a));
       a++;  
  }
   
   //  wordMap.put(32, imageArray[27]);
    
}

int currentWord;
boolean startPractice = false;
int correct = 0;
public void draw(){
  //background(#28A2D1);
   //println(int('a'));
   //image(wordMap.get(int('A')), 0, 0, pic_width/25, pic_height/25);
  //image(imageArray[0], 0, 0, 75.0, 75.0);
  
  if ( handMode) {
    // handIn = handIn + input ;
  
     
   if ( handIn.length() > 0) {   
      for ( int i = 0; i < 7; i++  ) {
         //wordMap.get(int('a')
         image(wordMap.get(int( handIn.charAt(i) )), 25 + (i * 90), 100, pic_width/20, pic_height/20);
       }
     if (handIn.length() > 7) {  
     for ( int i = 7; i < handIn.length(); i++  ) {
         //wordMap.get(int('a')
         image(wordMap.get(int( handIn.charAt(i) )), 25 + ( max(i-7,0) * 90), 220, pic_width/20, pic_height/20);
       }
       
     }
  
   }
    
  } // end of bool handMode
  
  if (startPractice) {
      if ( words[currentWord].length() <= handIn.length() ) {
          handIn = handIn + letter;
          
      }
    
     if (letter != null) {
     for ( int i = 0; i < 7; i++  ) {
         //wordMap.get(int('a')
        if( handIn.charAt(i) !=  words[currentWord].charAt(i) ) {
             stroke(#DE0202); 
        } else { 
            correct++;
            stroke(#12FC03);
        }
          
        rect(25 + (i * 90), 100, pic_width/20, pic_height/20);
       image(wordMap.get(int( handIn.charAt(i) )), 25 + (i * 90), 100, pic_width/20, pic_height/20);
       }
       if ( handIn.length() > 7) {
         for ( int i = 7; i < handIn.length(); i++  ) {
         //wordMap.get(int('a')
        image(wordMap.get(int( handIn.charAt(i) )), 25 + ( max(i-7,0) * 90), 220, pic_width/20, pic_height/20);
         }
       }
     
     }
  
    if ( handIn.length() == words[currentWord].length() ) {
       practiceLabel.setText( words[currentWord] + ": " + correct + " out of " + words[currentWord].length()  );
       
    }
    
  } // end of bool startPractice
  
  
} // end of draw()

// Use this method to add additional statements
// to customise the GUI controls
public void customGUI(){
      background(#28A2D1);
}

String letter;
void serialEvent (Serial myPort) 
{
  // get the ASCII string:
  delay(100);
  String inString = myPort.readStringUntil('\n');
  //println(inString);
  
  if (inString != null )
  {  
    
   // format:
   // header | up| down | right | left | --> 1 if pressed, 0 otherwise 
    
    
    letter = inString;
    
    
  }

}

String userInput;
String handIn;
boolean handMode = false;

void trainingMode () {
  
   background(#28A2D1);
    welcomeLabel.setVisible(false);
    trainingMode_button.setVisible(false);
    practiceMode_button.setVisible(false);
    back_button.setVisible(true);
    startButton.setVisible(false);
    practiceLabel.setVisible(false);
    modeLabel.setVisible(true);
    mode_text.setVisible(true);
    handInputButton.setVisible(true);
    trainingText.setVisible(true);
    clearButton.setVisible(true);
    welcomeLabel.setText("TRAINING MODE");
     
}

void practiceMode(){
   background(#28A2D1);
    welcomeLabel.setVisible(true);
    trainingMode_button.setVisible(false);
    practiceMode_button.setVisible(false);
    back_button.setVisible(true);
    
    practiceLabel.setVisible(true);
    startButton.setVisible(true);
     clearButton.setVisible(true);
     welcomeLabel.setText("PRACTICE MODE");
     
     
}


void mainMenu(){
   background(#28A2D1);
    welcomeLabel.setVisible(true);
    welcomeLabel.setText("POIS KEYBOARD TRAINING PROGRAM");
    trainingMode_button.setVisible(true);
    practiceMode_button.setVisible(true);
    
    // hide other labels
    back_button.setVisible(false);
    trainingText.setVisible(false);
  
   startButton.setVisible(false);
    practiceLabel.setVisible(false);
        modeLabel.setVisible(false);
    mode_text.setVisible(false);
    handInputButton.setVisible(false);
    clearButton.setVisible(false);
    
    
}
