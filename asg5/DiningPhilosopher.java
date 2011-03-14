//Main function
import java.io.*;
import java.util.*;

class DiningPhilosophers{

  public static void main(String[] args){
    File infile = new File(args[0]);
    Scanner in = null;
    try{
      in = new Scanner(infile);
    }
    catch(IOException e){
      System.out.println("File not found: " + e);
    }
    String line = in.nextLine();
    int num_philo = Integer.parseInt(line);
    Philosopher[] philosophers = new Philosopher[num_philo];
    Thread[] philo_threads = new Thread[num_philo];
    boolean right_handed = false;
    int think_time;
    int eat_time;
    Fork right, left;
    String[] tuple;
    for(int i = 0; i < num_philo; i++){
      if (i == 0){
        right = new Fork();
        left = new Fork();
      }
      else if (i != num_philo - 1){
        right = new Fork();
        Philosopher temp = philosophers[i-1];
        left = temp.get_left();
      }
      else{
        right = philosophers[0].get_left();
        left = philosophers[i-1].get_right();
      }
      if (i%2 == 0)
        right_handed = true;
      line = in.nextLine();
      tuple = line.split(" ");
      think_time = Integer.parseInt(tuple[0]);
      eat_time = Integer.parseInt(tuple[1]);
      philosophers[i] = new Philosopher(i,right_handed, right, 
                                   left,think_time, eat_time);
      philo_threads[i] = new Thread(philosophers[i]);
     }

     for (int j = 0; j < num_philo; j++)
       philo_threads[j].start();

  }

}
