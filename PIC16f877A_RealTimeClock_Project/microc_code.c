// Mehmet Kaan Erol - 170419512

// LCD modul baglanti tanimlamalari
sbit LCD_RS at RB2_bit;
sbit LCD_EN at RB3_bit;
sbit LCD_D4 at RB4_bit;
sbit LCD_D5 at RB5_bit;
sbit LCD_D6 at RB6_bit;
sbit LCD_D7 at RB7_bit;

sbit LCD_RS_Direction at TRISB2_bit;
sbit LCD_EN_Direction at TRISB3_bit;
sbit LCD_D4_Direction at TRISB4_bit;
sbit LCD_D5_Direction at TRISB5_bit;
sbit LCD_D6_Direction at TRISB6_bit;
sbit LCD_D7_Direction at TRISB7_bit;
// ---

char keypadPort at PORTD;

unsigned short read_ds1307(unsigned short address)
{
  unsigned short r_data;
  I2C1_Start();
  I2C1_Wr(0xD0);        // adres 0x68 ve ardindan yön biti (yazma için 0, okuma için 1) 0x68 ve ardindan 0 --> 0xD0
  I2C1_Wr(address);
  I2C1_Repeated_Start();
  I2C1_Wr(0xD1);
  r_data=I2C1_Rd(0);
  I2C1_Stop();
  return(r_data);
}

void write_ds1307(unsigned short address, unsigned short w_data)
{
  I2C1_Start();            // I2C baslatma sinyali verilir
                           // adres 0x68 ve ardindan yön biti (yazma için 0, okuma için 1) 0x68 ve ardindan 0 --> 0xD0
  I2C1_Wr(0xD0);           // I2C araciligiyla ir bayt gönder (cihaz adresi + W)
  I2C1_Wr(address);        // bayt gönder (DS1307 konumunun adresi)
  I2C1_Wr(w_data);         // veri gönder (veri yazilacak)
  I2C1_Stop();             // I2C durdurma sinyali gonder
}

unsigned char BCD2UpperCh(unsigned char bcd)
{
  return ((bcd >> 4) + '0');
}

unsigned char BCD2LowerCh(unsigned char bcd)
{
  return ((bcd & 0x0F) + '0');
}

int Binary2BCD(int a)
{
   int t1, t2;
   t1 = a%10;
   t1 = t1 & 0x0F;
   a = a/10;
   t2 = a%10;
   t2 = 0x0F & t2;
   t2 = t2 << 4;
   t2 = 0xF0 & t2;
   t1 = t1 | t2;
   return t1;
}

int BCD2Binary(int a)
{
   int r,t;
   t = a & 0x0F;
   r = t;
   a = 0xF0 & a;
   t = a >> 4;
   t = 0x0F & t;
   r = t*10 + r;
   return r;
}

short second = 0;
short minute = 0, minute1 = 1, minute2 = 32;
short hour   = 0, hour1 = 1, hour2 = 10;
short hr;
short ap;

unsigned short set_count = 0;
short set, durum_ayar = 1;

char time[]   = "00:00:00";
char temp_hour[] = " 00";
char temp_array[] = "Bas:00:00";
short hour1_array[10];
short min1_array[10];
short hour2_array[10];
short min2_array[10];
short durum_array[10];
short counter = 0;

short durum_kontrol(short hour, short minute)
{
 int i;
 set = 1;
 for (i=0;i<counter;i++)
 {
  if (hour>hour1_array[i] && hour<hour2_array[i]) set = durum_array[i];
  else if (hour==hour1 && minute>minute1)
  {
    if ((hour2==hour && minute2>=minute) || (hour2>hour)) set = durum_array[i];
  }
  else if (hour>hour1)
  {
    if ((hour2==hour && minute2>=minute) || (hour2>hour)) set = durum_array[i];
  }
 }
 return set;
}

void esas_saati_ayarla()
{
  second = read_ds1307(0);
  minute = read_ds1307(1);
  hour   = read_ds1307(2);
  hr     = hour & 0b00011111;
  ap     = hour & 0b00100000;

  /**  if (ap)
  {
   hour = BCD2Binary(hour);
   hour = hour + 12;
   hour = Binary2BCD(hour);
  }   **/

  time[0] = BCD2UpperCh(hour);
  time[1] = BCD2LowerCh(hour);
  time[2] = ':';
  time[3] = BCD2UpperCh(minute);
  time[4] = BCD2LowerCh(minute);
  time[5] = ':';
  time[6] = BCD2UpperCh(second);
  time[7] = BCD2LowerCh(second);

  Lcd_out(1,1,"Time:");
  Lcd_out(1, 6, time);

  // zaman araligini kontrol et
  if (durum_kontrol(BCD2Binary(hour), BCD2Binary(minute)) == 1)
  {
   Lcd_out(2, 1, "Acik  ");
   PORTD.RA0 = 1;
  }
  else
  {
   Lcd_out(2, 1, "Kapali");
   PORTD.RA0 = 0;
  }
}

short sub_counter = 0, check = 1, kp = 0;
void zaman_ayarla()
{
 hour1   = 1;
 minute1 = 1;
 hour2   = 1;
 minute2 = 1;
 durum_ayar = 1;
 Delay_ms(200);
 while (1)
 {
  // baslangic saatini gir
  if (Button(&PORTA, 0, 50, 1))
  {
   sub_counter = 0;
   Delay_ms(150);
  }

  // bitis saatini gir
  if (Button(&PORTA, 1, 50, 1))
  {
   sub_counter = 1;
   Delay_ms(150);
  }
  
    // saati arttir
  if (Button(&PORTD, 7, 50, 1))
  {
   if (sub_counter) hour2++;
   else hour1++;

   if (hour1 >= 60) hour1 = 0;
   else if (hour2 >= 60) hour2 = 0;
  }
  // dakikayi arttir
  if (Button(&PORTD, 6, 50, 1))
  {
   if (sub_counter) minute2++;
   else minute1++;
   
   if (minute1 >= 60) minute1 = 0;
   else if (minute2 >= 60) minute2 = 0;
  }

  // durumu degistir
  if (Button(&PORTA, 2, 50, 1))
  {
   if (durum_ayar) durum_ayar = 0;
   else durum_ayar = 1;
  }

  // cikis
  if (Button(&PortA, 3, 50, 1))
  {
    // kaydet ve cik
    if (hour1<hour2 || (hour1==hour2 && minute1<=minute2))
    {
      hour1_array[counter] = hour1;
      min1_array[counter]  = minute1;
      hour2_array[counter] = hour2;
      min2_array[counter]  = minute2;
      durum_array[counter] = durum_ayar;
      counter++;
      if (counter >= 10) counter = 0;
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_out(2, 1, "Kaydedildi");
    }
    else
    {
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_out(2, 1, "Gecersiz giris");
    }
    Delay_ms(1000);
    break;
  }

  temp_array[0] = 'B';
  temp_array[1] = 'a';
  temp_array[2] = 's';
  temp_array[3] = ':';
  if (hour1 < 10) temp_array[4] = '0';
  else temp_array[4] = '0' + (hour1/10);
  temp_array[5] = '0' + (hour1%10);
  temp_array[6] = ':';
  if (minute1 < 10) temp_array[7] = '0';
  else temp_array[7] = '0' + (minute1/10);
  temp_array[8] = '0' + (minute1%10);
  Lcd_out(1, 1, temp_array);

  temp_array[0] = 'B';
  temp_array[1] = 'i';
  temp_array[2] = 't';
  temp_array[3] = ':';
  if (hour2 < 10) temp_array[4] = '0';
  else temp_array[4] = '0' + (hour2/10);
  temp_array[5] = '0' + (hour2%10);
  temp_array[6] = ':';
  if (minute2 < 10) temp_array[7] = '0';
  else temp_array[7] = '0' + (minute2/10);
  temp_array[8] = '0' + (minute2%10);
  Lcd_out(2, 1, temp_array);

  if (durum_ayar == 1)
  {
   Lcd_out(3, 1, "Acik  ");
  }
  else
  {
   Lcd_out(3, 1, "Kapali");
  }
  Delay_ms(100);
 }
}

void main()
{
   I2C1_Init(100000); // DS1307 I2C 100KHz'de calisir

   CMCON  = 0x07;     // comparator'lari kapat
   ADCON1 = 0x06;     // a2d cevriciyi kapat

   TRISA = 0b11111111;
   PORTA = 0x00;

   TRISD = 0b11111110;
   PORTD = 0x0;

   Lcd_Init();                        // Initialize LCD
   Lcd_Cmd(_LCD_CLEAR);               // Clear display
   Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off

   Lcd_out(1,1,"Programlanabilir");
   Lcd_out(2,1,"Kombi Projesi");
   Delay_ms(2300);

   Lcd_Cmd(_LCD_CLEAR);
   Lcd_out(1,1,"Time:");
   Lcd_out(2,1,"Acik  ");
   // Lcd_out(4,1,"normal mod");

   while(1)
   {
     esas_saati_ayarla();

     if (Button(&PortA, 3, 10, 1))
     {
       Lcd_Cmd(_LCD_CLEAR);
       // Lcd_out(4,1,"zaman mod");
       zaman_ayarla();
       Lcd_Cmd(_LCD_CLEAR);
       // Lcd_out(4,1,"normal mod");
     }

     Delay_ms(100);
   }
}