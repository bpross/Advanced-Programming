import java.util.concurrent.Semaphore;

class Fork{
  private Semaphore available = new Semaphore(1);

  public void pickup(){
    try{
      available.acquire();
    }
    catch (InterruptedException e){
      throw new Error(e.toString());
    }
  }

  public void put_down(){
      available.release();
  }
}
