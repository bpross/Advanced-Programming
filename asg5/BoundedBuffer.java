import java.util.concurrent.ArrayBlockingQueue;

class BoundBuffer implements Buffer{
  private ArrayBlockingQueue<Integer> queue;
  
  BoundedBuffer(int size){
    queue = new ArrayBlockingQueue<Integer>(size);
  }
  
  public void put(int newValue){
    try{
      queue.put(newValue);
    }catch (InterruptedException e){
      throw new Error(e.toString() );
    }
  }

  public int get(){
    int result;
    try{
      result = queue.take();
    }catch(InterruptedException e){
      throw new Error(e.toSting() );
    }
    return result;
  }
}
