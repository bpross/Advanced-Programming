/*
   Erik Steggall, Ben Ross
   esteggal@ucsc.edu bpross@ucsc.edu
   cmps109
   winter 11
*/

class Professor implements Runnable{
  //fields
  private Buffer buffer;
  private bool thinking;
  private int forks;
  

  //constructor
  Professor(Buffer buffer, bool thinking, int forks){
    this.buffer = buffer;
    this.thinking = thinking;
    this.forks = forks;
  }

  public void run(){
    int x;
    system.out.println("Professor eating");
    for(int i = 0; i < 10; i++){
      x = buffer.get();
      System.out.println("Consumer has been eating " + x);
    }
    System.out.println("Professor is thinking");
   
  } 
