/*
   Erik Steggall, Ben Ross
   esteggal@ucsc.edu bpross@ucsc.edu
   cmps109
   winter 11
*/

class Philosopher implements Runnable{
  //fields
  private boolean right_handed;
  private Fork right;
  private Fork left;
  private int id;
  private int think_time;
  private int eat_time;
  private boolean has_right = false;
  private boolean has_left = false;
  

  //constructor
  Philosopher(int id, boolean right_handed, Fork right, Fork left, 
              int think_time,int eat_time){
    this.id = id;
    this.right_handed = right_handed;
    this.right = right;
    this.left = left;
    this.think_time = think_time;
    this.eat_time = eat_time;
  }

  public Fork get_left(){
    return this.left;
  }

  public Fork get_right(){
    return this.right;
  }

  public void run(){
    boolean finish_eat = false;
    boolean finish_think = false;
    while (!finish_eat && !finish_think){
      System.out.println("Philosopher" + id + ": started");
      if (!finish_eat){
        if (this.right_handed && this.right.available()){
          right.pickup();
          this.has_right = true;
        }
        if (!this.right_handed && this.left.available()){
          left.pickup();
          this.has_left = true;
        }
        if (this.has_right && this.left.available()){
          left.pickup();
          this.has_left = true;
        }
        if (this.has_left && this.right.available()){
          right.pickup();
          this.has_right = true;
        }
        if (right_handed && !this.right.available() && this.left.available()){
          left.pickup();
          this.has_left = true;
        }
        if (!right_handed && !left.available() && right.available()){
          right.pickup();
          this.has_right = true;
        }
      } 
   
      if (!has_right && !has_left){
        System.out.println("Philosopher" + id + ": thinking");
        try{
          Thread.currentThread().sleep(think_time);
        }
        catch(InterruptedException e){
          System.out.println("thinking interrupted: " + e);
        }
        System.out.println("Philosopher" + id + ": finished thinking");
        finish_think = true;
      }
      else if (has_right && has_left && !Thread.currentThread().isAlive()){
        System.out.println("Philosopher" + id + ": eating");
        try{
          Thread.currentThread().sleep(eat_time);
        }
        catch(InterruptedException e){
          System.out.println("eating interrupted: " + e);
        }
        System.out.println("Philosopher:" + id + ": finished eating");
        left.put_down();
        right.put_down();
        has_right = false;
        has_left = false;
        finish_eat = true;
      }
    }
    System.out.println("Philosopher" + id + ": finished");
  }
}
