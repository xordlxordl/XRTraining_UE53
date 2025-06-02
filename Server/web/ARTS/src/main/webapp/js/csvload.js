
const fileInput = document.getElementById('csv')
const previewCSVData = async dataurl => 
{
  const d = await d3.csv( dataurl);

  console.log(d);
  

}

const readFile = e => 
{
  const file = fileInput.files[0]
  const reader = new FileReader()
  reader.onload = () => 
  {
	  
    const dataUrl = reader.result;
    previewCSVData(dataUrl);
    
    //document.getElementById('out').textContent = reader.result;
  }
  reader.readAsDataURL(file)
}

fileInput.onchange = readFile;