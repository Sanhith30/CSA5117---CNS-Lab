# Stage 1: Build React Application
FROM node:18-alpine AS builder

WORKDIR /app

# Copy package lists and install dependencies
COPY package*.json ./
RUN npm install

# Copy application files and build
COPY . .
RUN npm run build

# Stage 2: Serve static files with Nginx
FROM nginx:1.25-alpine

# Copy built files from Builder stage
COPY --from=builder /app/dist /usr/share/nginx/html

# Copy default nginx configuration if needed, otherwise standard config is fine
# Expose port
EXPOSE 80

# Start Nginx
CMD ["nginx", "-g", "daemon off;"]
