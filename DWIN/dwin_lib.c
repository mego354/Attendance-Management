#include "Arduino.h"
#define batu_dwin_H

int department_name_length, college_name_length, subject_name_length, students_length, time_length, doctor_name_length;
int error_length = 8;
int refresh_rate = 60 * 5; //seconds * menutes
void GO_PAGE(uint16_t PG){ 
  Serial.write(0x5A);     Serial.write(0xA5);
  Serial.write(0x07);     Serial.write(0x82);
  Serial.write((byte)00); Serial.write(0x84);
  Serial.write(0x5A);     Serial.write(0x01);
  Serial.write(PG >> 8);  Serial.write(PG % 256);
}


void ERASE_TEXT(int high_address, long d_Len){ 
  Serial.write(0x5A); 
  Serial.write(0xA5); 
  Serial.write(d_Len + 3); 
  Serial.write(0x82);

  Serial.write(high_address);
  Serial.write(0x00);
  for (int t = 0; t < d_Len; t++) {
    Serial.write(0x20);
  }
}

void SET_TEXT(int high_address, String textData){ 
  int d_Len = textData.length();

  Serial.write(0x5A); 
  Serial.write(0xA5); 
  Serial.write(d_Len + 3); 
  Serial.write(0x82);
  Serial.write(high_address);
  Serial.write(0x00);
  for(int i = 0; i < d_Len; i++)
  {
    Serial.write(textData[i]);
  }
}

void conferm_case(String department_name, String college_name, String subject_name, String time, String students, int pic_id){
    GO_PAGE(1);
    delay(500);

    SET_TEXT(0x10, college_name);    SET_TEXT(0x20, department_name);
    SET_TEXT(0x30, subject_name);    SET_TEXT(0x40, students);
    SET_TEXT(0x50, time);
    GO_PAGE(pic_id);

    delay(1000 * refresh_rate);

    college_name_length = college_name.length();
    department_name_length = department_name.length();
    subject_name_length = subject_name.length();
    students_length = students.length();
    time_length = time.length();
    ERASE_TEXT(0x10, college_name_length);  ERASE_TEXT(0x20, department_name_length);
    ERASE_TEXT(0x30, subject_name_length);  ERASE_TEXT(0x40, students_length);
    ERASE_TEXT(0x50, time_length);
    delay(500);
}

void error_case(String error, int pic_id){
    ERASE_TEXT(0x80, error_length);
    if (error.length() > 100) {
        error = error.substring(0, 100);
    }
    error_length = error.length();
    SET_TEXT(0x80, error);
    GO_PAGE(pic_id);
    delay(1000 * refresh_rate);
    
    GO_PAGE(1);
    ERASE_TEXT(0x80, error_length);
    SET_TEXT(0x80, "starting");
    error_length = 8;

}
