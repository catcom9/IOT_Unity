var ButtonContents = document.getElementsByClassName("ButtonContent");

for(var i = 0; i != ButtonContents.length; i++){
    ButtonContents[i].addEventListener("mouseover", mouseEnterButton(ButtonContents[i]))
}


function mouseEnterButton(Element){
    Element.style.width = "auto";
}
