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
    Fork[] fork = new Fork[num_philo];
    for(int j =0;j<num_philo;j++)
      fork[j] = new Fork();
    Philosopher[] philosophers = new Philosopher[num_philo];
    Thread[] threads = new Thread[num_philo];
    //Pass in forks,think time and eat time
    int id = 1;
    for(int i=0;i<num_philo;i++){
      int neighbor = i+1;
      if (neighbor == num_philo)
        neighbor = 0;
      line = in.nextLine();
      String[] tuple = line.split(" ");
      int eat = Integer.parseInt(tuple[0]);
      int think = Integer.parseInt(tuple[0]);
      philosophers[i] = new Philosopher(id,fork[i],fork[neighbor],
                                        eat,think);
      System.out.println("ID: " + id +" EAT: " + eat + " THINK: " + think);
      threads[i] = new Thread(philosophers[i]);
      id++;
    }  
    //Start
    for(int k =0;k<num_philo;k++)
      threads[k].start();
  }
}
