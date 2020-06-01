package fileSearch20180445;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

class FileSearchWindow implements ItemListener{
    //생성
    public FileSearchWindow(){
        JFrame FileSearchWindow = new JFrame();

        JPanel panel1 = new JPanel();
        JPanel panel2 = new JPanel();
        JPanel panelBtn = new JPanel();

        Container fsearchFrame = FileSearchWindow.getContentPane();
        fsearchFrame.setLayout(new FlowLayout());

        panel1.add(new JLabel("FilePath : "));
        panel1.add(new JTextField(20));
        panel2.add(new JLabel("Keyword : "));
        panel2.add(new JTextField(20));
        panelBtn.add(new JButton("Search"));
        panelBtn.add(new JButton("End"));

        fsearchFrame.add(panel1);
        fsearchFrame.add(panel2);
        fsearchFrame.add(panelBtn);

        FileSearchWindow.setTitle("File Search - 20180445");
        FileSearchWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        FileSearchWindow.setSize(500,400);
        FileSearchWindow.setVisible(true);
    }

    public static void main(String[] args) {
        FileSearchWindow fsw = new FileSearchWindow();
    }

    @Override
    public void itemStateChanged(ItemEvent e) {

    }
}