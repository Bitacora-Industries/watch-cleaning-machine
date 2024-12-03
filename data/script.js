const toggleButton = document.getElementById("toggle-button");
const statusText = document.getElementById("status");

toggleButton.addEventListener("click", async () => {
  try {
    const response = await fetch(`/toggle`, { method: "GET" });
    const result = await response.text();
    statusText.textContent = `Status: ${result}`;
  } catch (error) {
    console.error("Error:", error);
    statusText.textContent = "Status: Error connecting to ESP32.";
  }
});
