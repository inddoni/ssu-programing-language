package fileSearch20180445;

public class MyException extends Exception{
    MyException(){};

    MyException(String msg){
        super(msg);
        System.out.println(msg);
    }
    MyException(Exception ex){
        super(ex);
    }
}
