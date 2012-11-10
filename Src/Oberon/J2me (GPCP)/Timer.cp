MODULE GrTiles;

private void sleep(){
      repaint();
      long startTime=System.currentTimeMillis();
      long timeTaken=System.currentTimeMillis()-startTime;
      while (timeTaken<50) {
         timeTaken=System.currentTimeMillis()-startTime;
      }
   }


END GrTiles.

