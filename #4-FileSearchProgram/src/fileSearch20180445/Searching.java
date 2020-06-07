package fileSearch20180445;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Searching extends JFrame {

    public Searching(){
        JPanel ta = new JPanel();
        JPanel btns = new JPanel();

        JTextArea textarea = new JTextArea(15, 22);
        ta.add(textarea);

        JButton fDelete = new JButton("Delete File");
        JButton sCancel = new JButton("Cancel Search");
        btns.add(fDelete);
        btns.add(sCancel);

        Container popupwindow = this.getContentPane();
        popupwindow.setLayout(new FlowLayout());

        popupwindow.add(ta);
        popupwindow.add(btns);

        fDelete.addActionListener(new sEventHandler());
        sCancel.addActionListener(new sEventHandler());

        this.setTitle("Searching... - 20180445");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(300,350);
        this.setVisible(true);
    }

    class sEventHandler implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent arg0) {//액션이벤트가 발생되었을떄 수행하는 동작
            if (arg0.getActionCommand() == "Delete File"){

            }
            else if (arg0.getActionCommand() == "Cancel Search"){

            }
            else {
                System.out.println("undefined");
            }
        }
    }
}
