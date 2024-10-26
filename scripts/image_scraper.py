import requests
from bs4 import BeautifulSoup
import os
from urllib.parse import urljoin

# Create a directory to store the images
output_dir = "images"
os.makedirs(output_dir, exist_ok=True)

# Target URL
url = "https://finalfantasy.fandom.com/wiki/Final_Fantasy_VIII_Triple_Triad_cards"

# Send a GET request to fetch the webpage content
response = requests.get(url)
response.raise_for_status()  # Ensure the request was successful

# Parse the HTML content with BeautifulSoup
soup = BeautifulSoup(response.content, "html.parser")

# Find all relevant tables with the class "full-width FFVIII article-table"
tables = soup.find_all("table", class_="full-width FFVIII article-table")

# Loop through each table and extract the images
for table in tables:
    # Find all <a> tags that contain the images
    image_links = table.find_all("a", class_="image")

    for link in image_links:
        # Extract the image URL from the 'href' attribute
        img_url = link.get("href")

        # Ensure the image URL is complete
        img_url = urljoin(url, img_url)

        # Get the image filename
        img_name = img_url.split("/")[-3]

        # Download the image
        print(f"Downloading {img_name}...")
        img_response = requests.get(img_url, stream=True)
        if img_response.status_code == 200:
            with open(os.path.join(output_dir, img_name), "wb") as f:
                for chunk in img_response.iter_content(1024):
                    f.write(chunk)
            print(f"Downloaded: {img_name}")
        else:
            print(f"Failed to download: {img_url}")

print("All images downloaded.")