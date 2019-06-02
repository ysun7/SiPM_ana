    ///##########
    
    
      for (k=1500;k<200000;k++)
  {
    
    if(h11->GetBinContent(k+1)>afterpulse_threshold[j])
    {
        baseline1=0;counter=0;
	for (m=k-1100;m<k-100;m++)
	{
	if(h11->GetBinContent(m+1)<afterpulse_threshold[j]){baseline1+=h11->GetBinContent(m+1);counter++;}
	}
	baseline1/=counter;
        if((k-lastpulse)<2000){baseline1=lastbaseline;}
	if((h11->GetBinContent(k+1)-baseline1)>afterpulse_threshold[j])
	{
	    ph=0;
	    for (m=k;m<k+200;m++)
	    {
	    if(h11->GetBinContent(m+1)>ph){ph=h11->GetBinContent(m+1);maxbin=m;}
	    }
	    height=ph-baseline1;

	afterpulse_height[j]->Fill(1000*(h11->GetBinContent(maxbin)-baseline1));
//cout<<"1 "<<k<<" "<<maxbin<<endl;
	integral=0;
    for (k1=maxbin;k1<200000;k1++)
  {
    if(h11->GetBinContent(k1+1)>baseline1)
    {
      integral+=h11->GetBinContent(k1+1)-baseline1;
    }
    else
    {
     jumpto=k1+1;break; //cout<<"2 "<<jumpto<<endl;
    }
  }
    for (k1=maxbin-1;k1>0;k1--)
  {
    if(h11->GetBinContent(k1+1)>baseline1)
    {
      integral+=h11->GetBinContent(k1+1)-baseline1;
    }
    else
    {
     break; 
    }
  }
      afterpulse_integral[j]->Fill(integral);
      f->cd();
      if(!flag){h11->SetName(Form("h%dV%d",j+23,afterpulse_counter));
      h11->Write();}
      afterpulse_counter++;
      cout<<"afterpulse found in event "<<i<<endl;
      pulse_time2=(k+1)/10.;
      afterpulse_time[j]->Fill((pulse_time2-pulse_time1)/1000.);
      lastpulse=k;
      k=jumpto;
      //cout<<"3 "<<k<<endl;
      flag=true;
      lastbaseline=baseline1;
	}
    }
  
  }
    
    
    ///###########