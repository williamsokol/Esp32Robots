const supa = require('@supabase/supabase-js');

const supabaseUrl = 'https://croqanlnwwpwfrajjgos.supabase.co';
const supabaseKey = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImNyb3Fhbmxud3dwd2ZyYWpqZ29zIiwicm9sZSI6ImFub24iLCJpYXQiOjE2ODU4MjQ1MjQsImV4cCI6MjAwMTQwMDUyNH0.yKni6eV7n4gKqjtC5uQgTLhCusLW3SymCWgub954upc';
const supabase = supa.createClient(supabaseUrl, supabaseKey);

async function AddRobot(name= 'a',password="b",id = null){
  if (id == null){
    id = await smallestUnusedID();
  }
  let date = new Date()
  let options = { hour: 'numeric', minute: 'numeric', hour12: true };
  let time = date.toLocaleTimeString('en-US', options);
  const tableName = 'ConnectedRobots';
  const data = [{ robot_name: 'value1', password: 'value2' , ID: id, time_created: time}];
  
  const { data: insertedData, error } = await supabase.from(tableName).insert(data);
  if (error) {
      console.error('Error inserting data:', error);
  } else {
      console.log('Data inserted successfully:', insertedData);
  }
}
async function readlist() 
{
    // Query the table for the newly added rows
    const { data, error } = await supabase
        .from('ConnectedRobots')
        .select('*')
        .order('robot_name', { ascending: false })
        //.limit(10); // Adjust the limit as per your requirements

    if (error) {
    console.error(error);
    } else {
    //console.log(data); // Newly added rows
    return data;
    }
}
async function CheckInMac(macAddr) 
{
    // Query the table for the newly added rows
    const { data, error } = await supabase
        .from('SavedRobots')
        .select('*')
        .eq("MAC", macAddr)
        // .order('robot_name', { ascending: false })
        //.limit(10); // Adjust the limit as per your requirements

    if (error) {
    console.error(error);
    } else {
      if(data.length == 0){
        
      }
    //console.log(data); // Newly added rows
    return data;
    }
}

async function deleteRow(id) {
  try {
    const { data, error } = await supabase
      .from('ConnectedRobots')
      .delete()
      .match({ ID : id }); // Specify the condition to match the row you want to delete

    if (error) {
      console.error(error);
      return;
    }

    console.log('Row deleted successfully:', data);
  } catch (error) {
    console.error('Error deleting row:', error.message);
  }
}
async function ClearTable() {
  try {
    const { data, error } = await supabase
      .from('ConnectedRobots')
      .delete('*').neq("time_created", 0) 

      
      //.match({ ID : id }); // Specify the condition to match the row you want to delete

    if (error) {
      console.error(error);
      return;
    }

    console.log('Row deleted successfully:', data);
  } catch (error) {
    console.error('Error deleting row:', error.message);
  }
}

async function smallestUnusedID()
{
    let IDs = [];
    let base = await readlist();
    for(let i=0;i<base.length;i++){
        IDs.push(base[i]['ID'])
    }
    // console.log(IDs);
    IDs.sort((a, b) => a - b)
    // console.log(IDs);
    for(let i=0;i<IDs.length;i++){
        if(i != IDs[i]){
            return i;
        }
    }
    return IDs.length;
}

module.exports = {
  supabase,
  AddRobot,
  readlist,
  deleteRow,
  ClearTable,
  smallestUnusedID,
};

