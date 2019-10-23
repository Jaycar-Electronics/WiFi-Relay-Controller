/*

this is the web-app javascript file

we are primarily using jquery to control because it makes
things easy and nice to read 

you can read more about jquery, including interactive demos, here:
https://www.w3schools.com/jquery/



*/

$('button.relay').on('click', function() {
  const button = $(this);

  const relay = button.attr('id');

  const mode = button.hasClass('isOff') ? 'activate' : 'deactivate';

  //when the button is clicked, activate the esp

  fetch(`/relay?relay=${relay}&mode=${mode}`).then(res => {
    if (res.status == 200) {
      button.toggleClass('isOn');
      button.toggleClass('isOff');
    } else {
      console.log(res.text);
    }
  });
});
