-- Enable the pgvector extension for similarity search
CREATE EXTENSION IF NOT EXISTS vector;

-- 1. Events Table
CREATE TABLE IF NOT EXISTS events (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(255) NOT NULL,
    description TEXT,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
);

-- 2. Photos Metadata Table (Stores GCS URLs)
CREATE TABLE IF NOT EXISTS photos (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    event_id UUID REFERENCES events(id) ON DELETE CASCADE,
    storage_url TEXT NOT NULL,
    is_quarantined BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
);

-- 3. Biometric Vectors Table (Subject to 30-day purge)
CREATE TABLE IF NOT EXISTS face_embeddings (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    photo_id UUID REFERENCES photos(id) ON DELETE CASCADE,
    event_id UUID REFERENCES events(id) ON DELETE CASCADE,
    embedding vector(512) NOT NULL, -- ONNX extracted vector
    bounding_box JSONB,             -- [x, y, width, height] of detected face
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
);

-- Create a Hierarchical Navigable Small World (HNSW) index for fast similarity lookups
CREATE INDEX IF NOT EXISTS idx_face_embeddings_vector
ON face_embeddings USING hnsw (embedding vector_cosine_ops);

-- 4. Bib Numbers Table (OCR extraction for marathons)
CREATE TABLE IF NOT EXISTS photo_bibs (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    photo_id UUID REFERENCES photos(id) ON DELETE CASCADE,
    event_id UUID REFERENCES events(id) ON DELETE CASCADE,
    bib_number VARCHAR(20) NOT NULL,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
);

-- Create a B-Tree index for instantaneous exact bib-number lookups
CREATE INDEX IF NOT EXISTS idx_photo_bibs_lookup
ON photo_bibs(event_id, bib_number);