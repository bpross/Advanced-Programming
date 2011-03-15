/*
   Erik Steggall, Ben Ross
   esteggal@ucsc.edu bpross@ucsc.edu
   cmps109
   winter 11
*/
import java.util.concurrent.Semaphore;

class Philosopher implements Runnable{
  //fields
  private int id;
  private Fork left_fork;
  private Fork right_fork;
  private int think_time;
  private int eat_time;

  public Philosopher(int i, Fork left, Fork right,
                     int think, int eat){
    id = id;
    left_fork = left;
    right_fork = right;
    think_time = think;
    eat_time = eat;
  }

  private void think(){
    System.out.println("Philosopher " + id + ": is thinking");
    try{
      Thread.currentThread().sleep(think_time);
    }
    catch (InterruptedException e){}
    System.out.println("Philosopher " + id + ": is done thinking");
  }

  private void eat(){
      right_fork.pickup();
      left_fork.pickup();
      System.out.println("Philosopher " + id + ": is starting to eat");
      try{
        Thread.currentThread().sleep(eat_time);
      }
      catch (InterruptedException e){}
      System.out.println("Philosopher " + id +": is done eating");
      right_fork.put_down();
      left_fork.put_down();
    }
  

  public void run(){
    think();
    eat();
  }
}
