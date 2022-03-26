var relays = [false, false, false, false, false, false, false, false];

/**
 * When a button is clicked it passes in the relay number and triggers the ESP.
 * @param {number} relay Starting at 0 - 7
 */
function toggleRelay(relay) {
  const mode = relays[relay] ? "activate" : "deactivate";
  const valueElement = document.getElementById(`value-${relay}`);

  fetch(`/relay?relay=${relay}&mode=${mode}`).then((res) => {
    if (res.status == 200) {
      // Flip the stored value
      relays[relay] = !relays[relay];
      valueElement.classList.toggle("on");
      valueElement.innerHTML = relays[relay] ? "On" : "Off";
    } else {
      console.log(res.text);
    }
  });
}
