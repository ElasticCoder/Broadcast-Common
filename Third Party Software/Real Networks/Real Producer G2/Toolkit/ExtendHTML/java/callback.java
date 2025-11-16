//callback.java
//Applet to test callback methods of embedded player.
//You will have to compile this using JavaC, and you will
//need to have the RMObserver, and the RAPlayer .class files
//in you classpath. you will also need the moz3_0.zip file
//in your class file.

import java.awt.*;
import java.applet.Applet;
import RMObserver;
import RAPlayer;
import netscape.javascript.JSException;
import netscape.javascript.JSObject;

//Class: callback
//Purpose: exercise callback methods of embedded player

public class callback extends Applet implements RMObserver{

	RAPlayer ra;
	Button author,title,copyright;
	
	//Method: callback.init()
	//Purpose: Initialize the applet
	public void init() {
		//setLayout(new BorderLayout() );
		//author = new Button("Set Author");
		//title = new Button("Set Title");
		//copyright = new Button("Set Copyright");
		//add("West",title);
		//add("Center",author);
		//add("East",copyright);

		JSObject window = JSObject.getWindow(this);
		JSObject embedded = (JSObject) window.getMember("document");

		//get handle to plugin
		ra = (RAPlayer)embedded.getMember("javaplug2");
		
		//register observer class to fire asynchronous callback events
		int tempPcks = ra.GetPktsTotal();

		ra.AdviseG2((RMObserver)this);	

		//handle URL events with applet instead of browser
		ra.SetAutoGoToURL(false);

		System.out.println("Callback applet initialized.");
	}

	public void onClipOpened(String shortClipname, String url){
		System.out.println("OnClipOpened sent."+ shortClipname +" "+ url);
	}

	public void onClipClosed(){
		System.out.println("OnClipClosed sent");
	}

	public void onShowStatus(String statustext){
		System.out.println("OnShowStatus() sent,"+ statustext);
	}

	public void onGoToURL(String url, String target){
		System.out.println("OnGoToURL sent,"+ url +" "+ target);
	}

	public boolean OnPositionChange(long newpos){
		System.out.println("OnPositionChange sent, the new position is "+ newpos
+"ms");
		return true;
	}

	public boolean OnLengthChange(long newlength){
		System.out.println("OnLengthChange sent, the new length is "+ newlength
+"ms");
		return true;
	}

	public boolean OnVolumeChange(short volume){
		System.out.println("OnVolumeChange sent, the new volume is "+ volume);
		return true;
	}

	public boolean OnTitleChange(String title){
		System.out.println("OnTitleChange sent, the new title is "+ title);
		return true;
	}

	public boolean OnAuthorChange(String author){
		System.out.println("OnAuthorChange sent, the new author is "+ title);
		return true;
	}

	public boolean OnCopyrightChange(String copyright){
		System.out.println("OnCopyrightChange sent, the new copyright is "+
copyright);
		return true;
	}

	public boolean OnClipSizeChange(long width, long height){
		System.out.println("OnClipSizeChange sent, height="+ height +" width="+
width);
		return true;
	}

	public boolean OnPlayStateChange(long newPlayState){
		System.out.println("OnPlayStateChange sent, the new play state is "+
newPlayState);
		return true;
	}

	public boolean OnErrorMessage(String text){
		System.out.println("OnErrorMessageBox sent with error: "+ text);
		return true;
	}

	public boolean OnStatsInfoChange(String text){
		System.out.println("OnStatsInfoChange sent with info: "+ text);
		return true;
	}

	public boolean OnPosLength(int Position, int Length){
		System.out.println("OnPosLength called, Position= "+ Position +" Length=
"+ Length);
		return true;
	}

	public boolean OnPresentationOpened(){
		System.out.println("OnPresentationOpened called");
		return true;
	}

	public boolean OnPresentationClosed(){
		System.out.println("OnPresentationClosed called");
		return true;
	}
	
	public boolean OnStatisticsChanged(){
		System.out.println("OnStatisticsChanged called");
		return true;
	}

	public boolean OnPreSeek(int OldTime,	int NewTime){
		System.out.println("OnPreSeek called, old time = "+ OldTime +", new time
= "+ NewTime);
		return true;
	}

	public boolean OnPostSeek(int OldTime, int NewTime){
		System.out.println("OnPostSeek called, old time = "+ OldTime +", new time
= "+ NewTime);
		return true;	
	}
	
	public boolean OnStop(){
		System.out.println("OnStop called");
		return true;
	}

	public boolean OnBuffering(int Flags, short PercentComplete){
		System.out.println("OnBuffering called, flags= "+ Flags +" percent
complete= "+PercentComplete);
		return true;
	}
		
	public boolean OnContacting(String HostName){
		System.out.println("OnContacting called, host name= "+HostName);
		return true;
	}

	public boolean OnResized(long lwidth, long lheight){
		System.out.println("OnResized sent, height= "+ lheight +" width= "+ lwidth);
		return true;
	}

	public boolean OnLButtonDown(int nFlags, int nX, int nY){
		System.out.println("OnLButtonDown sent, flags= "+ nFlags +" x= "+ nX +"
y= "+ nY);
		return true;
	}

	public boolean OnLButtonUp(int nFlags, int nX, int nY){
		System.out.println("OnLButtonUp sent, flags= "+ nFlags +" x= "+ nX +" y=
"+ nY);
		return true;
	}

	public boolean OnLButtonDblClk(int nFlags, int nX, int nY){
		System.out.println("OnLButtonDblClk sent, flags= "+ nFlags +" x= "+ nX +"
y= "+ nY);
		return true;
	}

	public boolean OnRButtonDown(int nFlags, int nX, int nY){
		System.out.println("OnRButtonDown sent, flags= "+ nFlags +" x= "+ nX +"
y= "+ nY);
		return true;
	}

	public boolean OnRButtonUp(int nFlags, int nX, int nY){
		System.out.println("OnRButtonUp sent, flags= "+ nFlags +" x= "+ nX +" y=
"+ nY);
		return true;
	}

	public boolean OnRButtonDblClk(int nFlags, int nX, int nY){
		System.out.println("OnRButtonDblClk sent, flags= "+ nFlags +" x= "+ nX +"
y= "+ nY);
		return true;
	}

	public boolean OnMouseMove(int nFlags, int nX, int nY){
		System.out.println("OnMouseMove sent, flags= "+ nFlags +" x= "+ nX +" y=
"+ nY);
		return true;
	}

	public boolean OnKeyDown(int nFlags, int nKey){
		System.out.println("OnKeyDown sent, flags= "+ nFlags +" key= "+ nKey);
		return true;
	}

	public boolean OnKeyUp(int nFlags, int nKey){
		System.out.println("OnKeyUp sent, flags= "+ nFlags +" key= "+ nKey);
		return true;
	}

	public boolean OnKeyPress(int nFlags, int nKey){
		System.out.println("OnKeyPress sent, flags= "+ nFlags +" key= "+ nKey);
		return true;
	}
	public boolean OnClipOpen(){
		System.out.println("OnClipOpen sent");
		return true;
	}
	public boolean OnKeyDown(int keycode){
		System.out.println("OnKeyDown sent with KeyCode equal "+ keycode);
		return true;
	}
	public boolean OnKeyUp(int keycode){
		System.out.println("OnKeyUp sent with KeyCode equal "+ keycode);
		return true;
	}
	public boolean OnKeyPress(int keycode){
		System.out.println("OnKeyPress sent with KeyCode equal "+ keycode);
		return true;
	}
	public boolean OnVolume(int newvolume){
		System.out.println("OnVolume sent with the new volume being "+ newvolume);
		return true;
	}
	public boolean OnMute(boolean muted){
		System.out.println("OnMute sent, Mute= "+ muted);
		return true;
	}
}