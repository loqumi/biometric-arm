#include <Servo.h> //подключаем библиотеку для работы с сервоприводами

//задаем мин. и макс. угол поворота для каждого сервопривода,
//при необходимости их можно исправлять здесь
#define MAX_THUMB1 160 //большой палец (пин 10) открыт
#define MIN_THUMB1 80 //большой палец (пин 10) закрыт
#define MAX_THUMB2 90 //большой палец (пин 9) открыт
#define MIN_THUMB2 30 //большой палец (пин 9) закрыт
#define MAX_INDEX_FINGER 160 //указательный палец (пин 6) закрыт
#define MIN_INDEX_FINGER 20 //указательный палец (пин 6) открыт
#define MAX_MIDDLE_FINGER 160 //средний палец (пин 5) закрыт
#define MIN_MIDDLE_FINGER 20 //средний палец (пин 5) открыт
#define MAX_LITTLE_RING_FINGER 160 //безымянный и мизинец (пин 3) открыт
#define MIN_LITTLE_RING_FINGER 20 //безымянный и мизинец (пин 3) закрыт

//задаем скорости движения пальцев
//при необходимости их можно исправлять здесь
#define SPEED_THUMB1 2
#define SPEED_THUMB2 2
#define SPEED_INDEX_FINGER 2
#define SPEED_MIDDLE_FINGER 3
#define SPEED_LITTLE_RING_FINGER 2

//создаем объеты для работы с каждым мотором
Servo servo_thumb1; //большой палец (Сервопривод Tower Pro MG90S, пин 10 на шилде)
Servo servo_thumb2; //большой палец (Сервопривод Tower Pro MG995, пин 9 на шилде)
Servo servo_index_finger; //указательный палец (Сервопривод Tower Pro MG995, пин 6 на шилде)
Servo servo_middle_finger; //средний палец (Сервопривод Tower Pro MG995, пин 5 на шилде)
Servo servo_little_ring_finfers; //безымянный и мизинец (Сервопривод Tower Pro MG995, пин 3 на шилде)

//переменные для хранения необработанных значаний ЭМГ-сигнала
int emg1 = 0;
int emg2 = 0;
//переменные для хранения обработанных значаний ЭМГ-сигнала (амплитуды)
int amp1 = 0;
int amp2 = 0;
//переменные для хранения пороговых значений (для определения сотояния мышцы - расслаблена/напряжена)
int threshold1 = 30;
int threshold2 = 30;
//перменные для хранения мин. и макс. значений при обработке
int max1 = 0;
int min1 = 255;
int max2 = 0;
int min2 = 255;
//служебный переменные для хранения угла поворота вала сервопривода (для каждого сервопривода) 
int t1 = 0;
int t2 = 0;
int t3 = 0;
int t4 = 0;
int t5 = 0;        

//функция для вычисления амплитуды ЭМГ-сигнала (обработанного значания)
void calc_amp() {                                                
  for (int k = 0; k < 32; k++) {   
    emg1 = analogRead(A0);
    emg2 = analogRead(A1);                                                                  
    emg1 = map(emg1, 0, 1023, 0, 255);
    emg2 = map(emg2, 0, 1023, 0, 255);     
    if (emg1 > max1)                      
      max1 = emg1;                       
    if (emg1 < min1)                     
      min1 = emg1;  

    if (emg2 > max2)                      
      max2 = emg2;                       
    if (emg2 < min2)                     
      min2 = emg2; 
  }
  amp1 =  0.5*amp1 + 0.5*(max1 - min1); 
  amp2 =  0.5*amp2 + 0.5*(max2 - min2); 
  //при необходимости данные строчки можно раскомментировать для проверки значений с датчиков
  Serial.print(amp1);
  Serial.print("     "); 
  Serial.println(amp2);               
  max1 = 0;                                  
  min1 = 255;  
  max2 = 0;                                  
  min2 = 255;                                 
}

void setup() {
  Serial.begin(115200);
  //указываем к каким контактам подключены сервоприводы
  servo_thumb1.attach(10);
  servo_thumb2.attach(9);
  servo_index_finger.attach(6);
  servo_middle_finger.attach(5);
  servo_little_ring_finfers.attach(3);
  //проверка работоспособности - все пальцы на руке один раз сжимаются и разжимаются 
  //цикл for выполняется 1 раз (если необходимо сделать несколько сжатий/разжатий, просто меняем условие: i < необходимое число)
  for (int i = 0; i < 1; i++){
  //все пальцы сжимаются, цикл while выполняется пока не досигнуты граничные значения угла поворота  
  while (servo_thumb1.read() > MIN_THUMB1 || 
         servo_thumb2.read() > MIN_THUMB2 || 
         servo_index_finger.read() < MAX_INDEX_FINGER || 
         servo_middle_finger.read() < MAX_MIDDLE_FINGER || 
         servo_little_ring_finfers.read() > MIN_LITTLE_RING_FINGER){
    //управление валом каждого сервопривода, знак + или - означает в какую сторону поворачивается вал
    t1 = servo_thumb1.read() - SPEED_THUMB1;
    servo_thumb1.write(t1);
    t2 = servo_thumb2.read() - SPEED_THUMB2;
    servo_thumb2.write(t2); 
    t3 = servo_index_finger.read() + SPEED_INDEX_FINGER;
    servo_index_finger.write(t3);
    t4 = servo_middle_finger.read() + SPEED_MIDDLE_FINGER;
    servo_middle_finger.write(t4);
    t5 = servo_little_ring_finfers.read() - SPEED_LITTLE_RING_FINGER;
    servo_little_ring_finfers.write(t5);
    delay(15);  
  }
  //все пальца разжимаются, цикл while выполняется пока не досигнуто граничное значения угла поворота
  while (servo_thumb1.read() < MAX_THUMB1 || 
         servo_thumb2.read() < MAX_THUMB2 || 
         servo_index_finger.read() > MIN_INDEX_FINGER || 
         servo_middle_finger.read() > MIN_MIDDLE_FINGER || 
         servo_little_ring_finfers.read() < MAX_LITTLE_RING_FINGER){
    //управление валом каждого сервопривода, знак + или - означает в какую сторону поворачивается вал
    t1 = servo_thumb1.read() + SPEED_THUMB1;
    servo_thumb1.write(t1);
    t2 = servo_thumb2.read() + SPEED_THUMB2;
    servo_thumb2.write(t2); 
    t3 = servo_index_finger.read() - SPEED_INDEX_FINGER;
    servo_index_finger.write(t3);
    t4 = servo_middle_finger.read() - SPEED_MIDDLE_FINGER;
    servo_middle_finger.write(t4);
    t5 = servo_little_ring_finfers.read() + SPEED_LITTLE_RING_FINGER;
    servo_little_ring_finfers.write(t5);
    delay(15);
    }
  }
}

void loop() {
  //вызываем функцию для вычисления амилитуды ЭМГ сигнала для двух датчиков
  calc_amp();
  //если обе мышцы напряжены (значения амплитуд больше пороговых значений)
  if (amp1 > threshold1 && amp2 > threshold2){
    //реализуем жест кулак - все пальцы прижаты к ладони
    //поворачиваем каждый сервопривод для реализации жеста
    //при этом углы поворота не должны выходить за мин. и макс. значения
    if (servo_thumb1.read() > MIN_THUMB1){ //80
      t1 = servo_thumb1.read() - SPEED_THUMB1; //2
      servo_thumb1.write(t1);
    }
    if (servo_thumb2.read() > MIN_THUMB2){
      t2 = servo_thumb2.read() - SPEED_THUMB2;
      servo_thumb2.write(t2); 
    }
    if(servo_index_finger.read() < MAX_INDEX_FINGER){
      t3 = servo_index_finger.read() + SPEED_INDEX_FINGER;
      servo_index_finger.write(t3);
    }
    if (servo_middle_finger.read() < MAX_MIDDLE_FINGER){
      t4 = servo_middle_finger.read() + SPEED_MIDDLE_FINGER;
      servo_middle_finger.write(t4);
    }
    if (servo_little_ring_finfers.read() > MIN_LITTLE_RING_FINGER){
      t5 = servo_little_ring_finfers.read() - SPEED_LITTLE_RING_FINGER;
       servo_little_ring_finfers.write(t5);
    }          
  } 
  //если только 1 мышца напряжена (значение амплитуды больше порогового значения)
  else if (amp1 > threshold1){
    //реализуем жест V - прижаты все, кроме указательного и среднего
    //поворачиваем каждый сервопривод для реализации жеста
    //при этом углы поворота не должны выходить за мин. и макс. значения
    if (servo_thumb1.read() > MIN_THUMB1){
      t1 = servo_thumb1.read() - SPEED_THUMB1;
      servo_thumb1.write(t1);
    }
    if (servo_thumb2.read() > MIN_THUMB2){
      t2 = servo_thumb2.read() - SPEED_THUMB2;
      servo_thumb2.write(t2); 
    }
    if(servo_index_finger.read() > MIN_INDEX_FINGER){
      t3 = servo_index_finger.read() - SPEED_INDEX_FINGER;
      servo_index_finger.write(t3);
    }
    if (servo_middle_finger.read() > MIN_MIDDLE_FINGER){
      t4 = servo_middle_finger.read() - SPEED_MIDDLE_FINGER;
      servo_middle_finger.write(t4);
    }
    if (servo_little_ring_finfers.read() > MIN_LITTLE_RING_FINGER){
      t5 = servo_little_ring_finfers.read() - SPEED_LITTLE_RING_FINGER;
      servo_little_ring_finfers.write(t5);
    }          
  }
  //если только 2 мышца напряжена (значение амплитуды больше порогового значения)
  else if (amp2 > threshold2){
    //реализуем жест ОК - все пальцы разжаты, кроме большого и указательного
    //поворачиваем каждый сервопривод для реализации жеста
    //при этом углы поворота не должны выходить за мин. и макс. значения
    if (servo_thumb1.read() > MIN_THUMB1){
      t1 = servo_thumb1.read() - SPEED_THUMB1;
      servo_thumb1.write(t1);
    }
    if (servo_thumb2.read() > MIN_THUMB2){
      t2 = servo_thumb2.read() - SPEED_THUMB2;
      servo_thumb2.write(t2); 
    }
    if(servo_index_finger.read() < MAX_INDEX_FINGER){
      t3 = servo_index_finger.read() + SPEED_INDEX_FINGER;
      servo_index_finger.write(t3);
    }
    if (servo_middle_finger.read() > MIN_MIDDLE_FINGER){
      t4 = servo_middle_finger.read() - SPEED_MIDDLE_FINGER;
      servo_middle_finger.write(t4);
    }
    if (servo_little_ring_finfers.read() < MAX_LITTLE_RING_FINGER){
      t5 = servo_little_ring_finfers.read() + SPEED_LITTLE_RING_FINGER;
      servo_little_ring_finfers.write(t5);
    }       
  }
  //иначе - все пальцы остаются разжаты или разжимаются, в зависимости от текущего положения валов сервоприводов
  else{
    //реализуем жест ладонь - все пальцы разжаты
    //поворачиваем каждый сервопривод для реализации жеста
    //при этом углы поворота не должны выходить за мин. и макс. значения
    if (servo_thumb1.read() < MAX_THUMB1){
      t1 = servo_thumb1.read() + SPEED_THUMB1;
      servo_thumb1.write(t1);
    }
    if (servo_thumb2.read() > MIN_THUMB2){
      t2 = servo_thumb2.read() - SPEED_THUMB2;
      servo_thumb2.write(t2); 
    }
    if(servo_index_finger.read() > MIN_INDEX_FINGER){
      t3 = servo_index_finger.read() - SPEED_INDEX_FINGER;
      servo_index_finger.write(t3);
    }
    if (servo_middle_finger.read() > MIN_MIDDLE_FINGER){
      t4 = servo_middle_finger.read() - SPEED_MIDDLE_FINGER;
      servo_middle_finger.write(t4);
    }
    if (servo_little_ring_finfers.read() > MIN_LITTLE_RING_FINGER){
      t5 = servo_little_ring_finfers.read() - SPEED_LITTLE_RING_FINGER;
      servo_little_ring_finfers.write(t5);
    }
  }
  delay(10);
}
