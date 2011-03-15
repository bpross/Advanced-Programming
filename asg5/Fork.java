import java.util.concurrent.Semaphore;

class Fork{
  private Semaphore available = new Semaphore(1);

/* Symbolizes philosopher picking up a fork */
  public void pickup(){
    try{
      available.acquire();
    }
    catch (InterruptedException e){
      throw new Error(e.toString());
    }
  }

/* Symbolizes putting a fork down */
  public void put_down(){
      available.release();
  }
}
