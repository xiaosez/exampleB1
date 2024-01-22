void draw(){
	TFile *f = TFile::Open("/lustre/collider/yanzelin/tmp/learncpp/G4B1_p_momentum.root");
	if(!f->IsOpen()){
		cout<<"File not open"<<endl;
		return;
	}
	TTree *t = (TTree*)f->Get("tree");
	if(!t){
		cout<<"Tree not open"<<endl;
		return;
	}
	
	
	TFile *fout = new TFile("G4B1_plot.root","RECREATE");
	TH1D *h1 = new TH1D("h1","G4B1_p_momentum",50,441.5,442.3);
	static double momentum;
	t->SetBranchAddress("p_momentum",&momentum);
	cout << "circle begin" << endl;
	for(int i=0;i<t->GetEntries();i++){
		if(i%1000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		
		t->GetEntry(i);
		h1->Fill(momentum);
		if(i%50==0) cout<<"momentum is:" << momentum << endl;
	}
	cout << "total entries:"  << t->GetEntries() << endl;
	fout->cd();
	h1->Write();
	fout->Close();


}
