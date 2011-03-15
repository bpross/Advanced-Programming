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

/* Philosopher constructor */
  public Philosopher(int id, Fork left, Fork right,
                     int think, int eat){
    this.id = id;
    this.left_fork = left;
    this.right_fork = right;
    this.think_time = think;
    this.eat_time = eat;
  }

/* This function will sleep for the time that the philosopher is "thinking */
  private void think(){
    System.out.println("Philosopher " + id + ": is thinking");
    /* Check for interrupts */
    try{
      Thread.currentThread().sleep(think_time);
    }
    catch (InterruptedException e){}
    System.out.println("Philosopher " + id + ": is done thinking");
  }

/* This function will sleep for the time that the philosopher is "eating */
  private void eat(){
      /* Attempt to pick up both forks */
      right_fork.pickup();
      left_fork.pickup();
      System.out.println("Philosopher " + id + ": is starting to eat");
      /* Check for interrupts */
      try{
        Thread.currentThread().sleep(eat_time);
      }
      catch (InterruptedException e){}
      System.out.println("Philosopher " + id +": is done eating");
      right_fork.put_down();
      left_fork.put_down();
    }
  
/* The philosopher's actions */
  public void run(){
    think();
    eat();
  }
}
