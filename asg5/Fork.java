class Fork{
  private boolean available = true;
  private int id;

  public synchronized void pickup(){
    if(available){
      available = false;
    }
  }

  public synchronized void put_down(){
    available = true;
  }

  public boolean available(){
    return available;
  }
}
