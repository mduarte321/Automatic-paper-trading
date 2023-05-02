#include "Learner.h"

//#include "doublefann.h"
//#include "fann.h"

void TrainNN(StockCollection& sc) {

  //////////////////////////////////////////////////////////////
  //make the data file
  vector<Stock>& allStocks = sc.GetAllStocks();
  vector< vector<double> > allInputs;
  vector< double > allOutputs;

  for(int i=0; i<(int)allStocks.size(); i++) {
    allStocks[i].PrepareNNData( allInputs, allOutputs );
  }//endfor i

  string fileName = "NNstock.data";
  string fileName2 = "NNstock.data.csv";
  string outfileName = fileName + ".net";
  ofstream outfile;
  ofstream outfile2;
  outfile.open( fileName.c_str() );
  outfile2.open( fileName2.c_str() );
  outfile << allInputs.size() << " " << allInputs[0].size() << " 1" << endl;
  for(int i=0; i<(int)allInputs.size(); i++) {
    for(int j=0; j<(int)allInputs[i].size(); j++) {
      outfile << allInputs[i][j]  << " ";
      outfile2 << allInputs[i][j] <<",";
      //if( j < (allInputs[i].size()-1) ) 
      // 	outfile2<< ",";
      //else 
      //	outfile << endl;
    }//endfor j
    outfile << endl;
    outfile << allOutputs[i] << endl;
    outfile2 << allOutputs[i] << endl;
  }//endfor i
  outfile.close();
  outfile2.close();

  if(0) {
    cout << "BAILING early on learning! Skip!!! LOOK FOR THIS TO return." << endl;
    return;
  }
/*
  //////////////////////////////////////////////////////////////
  //train the neural network
  //const unsigned int num_input = 2;
  unsigned int num_input = allInputs[0].size();
  const unsigned int num_output = 1;
  //const unsigned int num_layers = 3;
  const unsigned int num_layers = 4;
  //const unsigned int num_neurons_hidden = 3;
  //const unsigned int num_neurons_hidden = 400;
  //const unsigned int num_neurons_hidden = 200;
  const unsigned int num_neurons_hidden = 400;
  const float desired_error = (const float) 0.001;
  //const unsigned int max_epochs = 500000;
  const unsigned int max_epochs = 50000;
  const unsigned int epochs_between_reports = 1000;

  //struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
  //struct fann *ann = fann_create_standard(num_layers, num_input, 18, 19, 1);
  //struct fann *ann = fann_create_standard(6, num_input, 18, 19, 18, 19, 1);
  //struct fann *ann = fann_create_standard(16, num_input, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1);
  //struct fann *ann = fann_create_standard(16, num_input, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1);
  //struct fann *ann = fann_create_standard(16, num_input, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 1);
  struct fann *ann = fann_create_standard(16, num_input, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 1);

  //fann_set_training_algorithm(ann, FANN_TRAIN_INCREMENTAL);
  fann_set_train_error_function(ann, FANN_ERRORFUNC_LINEAR);
  fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
  fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

  //changing default
  //fann_set_learning_rate(ann, 0.02);
  fann_set_learning_rate(ann, 0.08);
  fann_print_connections(ann);
  fann_print_parameters(ann);

  //fann_train_on_file(ann, "xor.data", max_epochs, epochs_between_reports, desired_error);
  fann_train_on_file(ann, fileName.c_str(), max_epochs, epochs_between_reports, desired_error);

  fann_print_connections(ann);
  //fann_save(ann, "xor_float.net");
  fann_save(ann, outfileName.c_str());

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // This is a test of the recent evaluation function... copying code from there to see...
  cout << "TESTING CODE --> from func: RecentEvaluation - numStocks: " << sc.GetAllStocks().size() << endl;
  //////////////////////////////////////////////////////////////
  //make neural network from file
  //string fileName = "NNstock.data";
  //string networkFileName = fileName + ".net";

  //struct fann *ann = fann_create_from_file(networkFileName.c_str());
  //fann_set_training_algorithm(ann, FANN_TRAIN_INCREMENTAL);
  //fann_set_train_error_function(ann, FANN_ERRORFUNC_LINEAR);
  //fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
  //fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

  //changing default
  //fann_set_learning_rate(ann, 0.02);
  //fann_set_learning_rate(ann, 0.08);
  //fann_print_connections(ann);
  fann_print_parameters(ann);

  //////////////////////////////////////////////////////////////
  //for all stocks, get their last N indicators
  //and pass them through the neural network
  //this already exists -> vector<Stock>& allStocks = sc.GetAllStocks();


  for(int i=0; i<(int)allStocks.size(); i++) { // for each stock
    cout << "i = " << i << endl;
    Stock& stock = allStocks[i];
    int numDayInfo = stock.GetNumDayInfo();
    cout << "numDayInfo = " << numDayInfo << endl;
    if( (numDayInfo-10) <= 0) continue;
    //for each of the last days, run through nn
    for(int j= numDayInfo-1; j>=numDayInfo-10; j--) { 
      cout << "j = " << j << endl;

      fann_type *calc_out;
      fann_type input[19];

      DayInfo& di = stock.GetDayInfo(j);
      vector<double> values;
      di.SetNNInputs(values);
      cout << "input | ";
      for(int k=0; k<values.size(); k++) {
	input[k] = values[k];
        cout << input[k] << " " ;
      }//endfor k
      cout << "|"<<endl;
      calc_out = fann_run(ann, input);
      cout << stock.GetSymbol() << " " << di.GetDate() << " calc_out = " << *calc_out << endl;
      di.SetNNScore( *calc_out, true );
    }//endfor j
  }//endfor i

  fann_destroy(ann);
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////

  fann_destroy(ann);
*/
}


void RecentEvaluation(StockCollection& sc) {

  cout << "RecentEvaluation - numStocks: " << sc.GetAllStocks().size() << endl;
  //////////////////////////////////////////////////////////////
  //make neural network from file
  string fileName = "NNstock.data";
  string networkFileName = fileName + ".net";

/*
  struct fann *ann = fann_create_from_file(networkFileName.c_str());
  //fann_set_training_algorithm(ann, FANN_TRAIN_INCREMENTAL);
  fann_set_train_error_function(ann, FANN_ERRORFUNC_LINEAR);
  fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
  fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

  //changing default
  //fann_set_learning_rate(ann, 0.02);
  fann_set_learning_rate(ann, 0.08);
  fann_print_connections(ann);
  fann_print_parameters(ann);

  //////////////////////////////////////////////////////////////
  //for all stocks, get their last N indicators
  //and pass them through the neural network
  vector<Stock>& allStocks = sc.GetAllStocks();


  for(int i=0; i<(int)allStocks.size(); i++) { // for each stock
    cout << "i = " << i << endl;
    Stock& stock = allStocks[i];
    int numDayInfo = stock.GetNumDayInfo();
    cout << "numDayInfo = " << numDayInfo << endl;
    if( (numDayInfo-10) <= 0) continue;
    //for each of the last days, run through nn
    for(int j= numDayInfo-1; j>=numDayInfo-10; j--) { 
      cout << "j = " << j << endl;

      fann_type *calc_out;
      fann_type input[19];

      DayInfo& di = stock.GetDayInfo(j);
      vector<double> values;
      di.SetNNInputs(values);
      cout << "input | ";
      for(int k=0; k<values.size(); k++) {
	input[k] = values[k];
        cout << input[k] << " " ;
      }//endfor k
      cout << "|"<<endl;
      calc_out = fann_run(ann, input);
      cout << stock.GetSymbol() << " " << di.GetDate() << " calc_out = " << *calc_out << endl;
      di.SetNNScore( *calc_out, true );
    }//endfor j
  }//endfor i

  fann_destroy(ann);
*/
  
}
