//Main function
import java.io.*;
import java.util.*;

class DiningPhilosophers{

  public static void main(String[] args){

    /* Create scanner for file parsing */
    File infile = new File(args[0]);
    Scanner in = null;
    /* make sure there is a filename given */
    try{
      in = new Scanner(infile);
    }
    catch(IOException e){
      System.out.println("File not found: " + e);
    }
    /* Parse file */
    String line = in.nextLine();
    int num_philo = Integer.parseInt(line);
    /* Create forks */
    Fork[] fork = new Fork[num_philo];
    for(int j =0;j<num_philo;j++)
      fork[j] = new Fork();
    /* Create array of Philosophers */
    Philosopher[] philosophers = new Philosopher[num_philo];
    /* Create array of threads */
    Thread[] threads = new Thread[num_philo];
    //Pass in forks,think time and eat time
    int id = 1;
    /* This loop initializes the philosophers */
    for(int i=0;i<num_philo;i++){
      /* Neighbor is next philosopher */
      int neighbor = i+1;
      /* If at last philosopher, no neighbor */
      if (neighbor == num_philo)
        neighbor = 0;
      /* Parse next line of infile for eat/think times*/
      line = in.nextLine();
      String[] tuple = line.split(" ");
      int eat = Integer.parseInt(tuple[0]);
      int think = Integer.parseInt(tuple[0]);
      /* Initialize new philosopher */
      philosophers[i] = new Philosopher(id,fork[i],fork[neighbor],
                                        eat,think);
      /* Initialize new thread */
      threads[i] = new Thread(philosophers[i]);
      id++;
    }  
    //Start
    for(int k =0;k<num_philo;k++)
      threads[k].start();
  }
}
